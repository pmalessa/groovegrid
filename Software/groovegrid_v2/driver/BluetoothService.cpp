/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "BluetoothService.h"

bool BluetoothService::isConnected()
{
	return connected;
}

void BluetoothService::onConnect()
{
	connected = true;
}

void BluetoothService::onDisconnect()
{
	connected = false;
}

BluetoothService& BluetoothService::getInstance()
{
	static BluetoothService _instance;
	return _instance;
}

BluetoothService::~BluetoothService(){}
BluetoothService::BluetoothService()
{
	CommChannel *ch;
	Serial.begin(115200);
	Serial.print("Hey!\n");

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
		BluetoothAdvertiser->addServiceUUID(*ch->serviceUUID);
	}

	BluetoothAdvertiser->setScanResponse(true);
	BluetoothAdvertiser->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	BluetoothAdvertiser->setMinPreferred(0x12);
	BLEDevice::startAdvertising();

}

std::string BluetoothService::onRead(uint8_t channelID)
{
	Serial.print("Read on Channel ");
	Serial.println(channelID);

	return "0";
}

void BluetoothService::onWrite(std::string data, uint8_t channelID)
{
	DynamicJsonDocument doc(200);

	Serial.print("Write on Channel ");//call CommListeners
	Serial.print(channelID);
	Serial.print(": ");
	Serial.println(data.c_str());

	DeserializationError error = deserializeJson(doc, data);
	if (error) {
	Serial.print(F("deserializeJson() failed: "));
	Serial.println(error.c_str());
	return;
	}

	channelList.at(channelID)->commInterface->onCommand(doc, channelID);	//parse doc to app
}

void BluetoothService::Attach(CommInterface *callbackPointer, ChannelID channelID)
{
    for (uint16_t i=0; i < channelList.size(); i++) {
        if(channelList.at(i)->channelID == channelID)
        {
            channelList.at(i)->commInterface = callbackPointer;
        }
    }
}

void BluetoothService::sendResponse(DynamicJsonDocument doc, uint8_t channelID)
{
	String Output;
	serializeJson(doc, Output);
	channelList[channelID]->txCharacteristic->setValue(Output.c_str());
	channelList[channelID]->txCharacteristic->notify(true);
}

void BluetoothService::run()
{
}
