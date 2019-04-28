/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "COMM.h"

bool COMM::isConnected()
{
	return connected;
}

void COMM::onConnect()
{
	connected = true;
}

void COMM::onDisconnect()
{
	connected = false;
}

COMM& COMM::getInstance()
{
	static COMM _instance;
	return _instance;
}

COMM::~COMM(){}
COMM::COMM()
{
	CommChannel *ch;
	Serial.begin(115200);
	Serial.print("Hey!\n");

	channelList.setStorage(channelstorage, MAX_CHANNEL_NUM, 0);

	/*SERVER*/
	BLEDevice::init("GrooveGrid");
	BluetoothServer = BLEDevice::createServer();
	BluetoothServer->setCallbacks(new CommServerCallback(this));
	BluetoothAdvertiser = BLEDevice::getAdvertising();

	/*CONTROL*/
	ch = new CommChannel;
	ch->channelID = CHANNEL_CONTROL;
	ch->channelName = "Control";
	ch->serviceUUID = new BLEUUID(SERVICE_CONTROL_UUID);
	ch->rxUUID = new BLEUUID(RX_UUID);
	ch->txUUID = new BLEUUID(TX_UUID);
	channelList.push_back(ch);

	/*USER1*/
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER1;
	ch->channelName = "User1";
	ch->serviceUUID = new BLEUUID(SERVICE_USER1_UUID);
	ch->rxUUID = new BLEUUID(RX_UUID);
	ch->txUUID = new BLEUUID(TX_UUID);
	channelList.push_back(ch);

	/*USER2*/
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER2;
	ch->channelName = "User2";
	ch->serviceUUID = new BLEUUID(SERVICE_USER2_UUID);
	ch->rxUUID = new BLEUUID(RX_UUID);
	ch->txUUID = new BLEUUID(TX_UUID);
	channelList.push_back(ch);

	//Init all Channels at BLE
	for (uint16_t i=0; i < channelList.size(); i++) {
		ch = channelList.at(i);
		ch->attachedService = BluetoothServer->createService(*ch->serviceUUID);
		ch->rxCharacteristic = ch->attachedService->createCharacteristic(*ch->rxUUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_NOTIFY);
		ch->txCharacteristic = ch->attachedService->createCharacteristic(*ch->txUUID,BLECharacteristic::PROPERTY_WRITE);
		ch->rxCharacteristic->setCallbacks(new CommCharacteristicCallback(this, ch->channelID));
		ch->txCharacteristic->setCallbacks(new CommCharacteristicCallback(this, ch->channelID));
		ch->attachedService->start();
		ch->commInterface = (CommInterface *)nullptr;
		BluetoothAdvertiser->addServiceUUID(*ch->serviceUUID);
	}

	BluetoothAdvertiser->setScanResponse(true);
	BluetoothAdvertiser->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	BluetoothAdvertiser->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
}

void COMM::Attach(CommInterface *callbackPointer, ChannelID channel)
{
	for (uint16_t i=0; i < channelList.size(); i++) {
		if(channelList.at(i)->channelID == channel)
		{
			channelList.at(i)->commInterface = callbackPointer;
		}
	}
}

void COMM::Detach(CommInterface *callbackPointer)
{
	for (uint16_t i=0; i < channelList.size(); i++) {
		if(channelList.at(i)->commInterface == callbackPointer)
		{
			//channelList.at(i)->commListener = (void *)0;	No detach necessary, attach does overwrite previous attach
		}
	}
}

std::string COMM::onRead(uint8_t channelID)
{
	Serial.print("Read on Channel ");
	Serial.println(channelID);

	for (uint16_t i=0; i < channelList.size(); i++)
	{
		if(channelList.at(i)->channelID == channelID)
		{
			if(channelList.at(i)->commInterface != nullptr)
			{
				return channelList.at(i)->commInterface->onUserRead(channelID); //call CommListeners
			}
		}
	}

	return "0";
}
void COMM::onWrite(std::string data, uint8_t channelID)
{
	Serial.print("Write on Channel ");//call CommListeners
	Serial.print(channelID);
	Serial.print(": ");
	Serial.println(data.c_str());

	for (uint16_t i=0; i < channelList.size(); i++)
	{
		if(channelList.at(i)->channelID == channelID)
		{
			if(channelList.at(i)->commInterface != nullptr)
			{
				channelList.at(i)->commInterface->onUserWrite(data, channelID); //call CommListeners
			}
		}
	}
}

void COMM::run()
{
}
