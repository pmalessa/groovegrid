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

	for(uint8_t i=0;i<MAX_USERS;i++)
	{
		connectedUsers[i] = false;
	}

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

	/*USER3*/
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER3;
	ch->channelName = "User3";
	ch->serviceUUID = new BLEUUID(SERVICE_USER3_UUID);
	ch->rxUUID = new BLEUUID(RX_UUID);
	ch->txUUID = new BLEUUID(TX_UUID);
	channelList.push_back(ch);

	/*USER4*/
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER4;
	ch->channelName = "User4";
	ch->serviceUUID = new BLEUUID(SERVICE_USER4_UUID);
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
		ch->txdescriptor = new BLE2902();
		ch->txdescriptor->setNotifications(1);
		ch->txCharacteristic->addDescriptor(ch->txdescriptor);
		ch->rxdescriptor = new BLE2902();
		ch->rxdescriptor->setNotifications(1);
		ch->rxCharacteristic->addDescriptor(ch->rxdescriptor);
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
	DynamicJsonDocument doc(200), rspDoc(200);
	uint8_t errorCode = 0;

	Serial.print("Write on Channel ");
	Serial.print(channelID);
	Serial.print(": ");
	Serial.println(data.c_str());

	DeserializationError error = deserializeJson(doc, data);
	if (error)
	{
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
		rspDoc["error"]= error.c_str();				//add error
		sendResponse(rspDoc, channelID);			//send Response
		return;										//leave if error
	}

	//----------BLE Command Parsing---------------
	rspDoc["rspID"] = doc["cmdID"];	//send cmdID back as rspID

	//CONNECT CMD
	if(doc["cmd"] == "connect")
	{
		uint8_t userID = doc["userID"];
		if(userID < MAX_USERS)
		{
			if(connectedUsers[userID] != true)
			{
				connectedUsers[userID] = true; //slot is free, connect allowed
				channelList.at(userID+1)->commInterface->onCommand(doc, channelID);	//channelID= userID+1, notify game
				errorCode = 0;//send success response
			}
			else
			{
				errorCode = 1;//send error response
			}
		}
		else
		{
			errorCode = 2;//send error response
		}
	}
	//DISCONNECT CMD
	else if(doc["cmd"] == "disconnect")
	{
		uint8_t userID = doc["userID"];
		if(userID < MAX_USERS)
		{
			if(connectedUsers[userID] == true)
			{
				connectedUsers[userID] = false; //slot is used, disconnect allowed
				channelList.at(userID+1)->commInterface->onCommand(doc, channelID);	//channelID= userID+1, notify game
				errorCode = 0;//send success response
			}
			else
			{
				errorCode = 1;//send error response
			}
		}
		else
		{
			errorCode = 2;//send error response
		}
	}
	//GET USER IDs CMD
	else if(doc["cmd"] == "getUserIDs")
	{
		//send userIDs
		JsonArray usersArray = rspDoc.createNestedArray("userIDs");
		for(uint8_t i=0;i<MAX_USERS;i++)
		{
			usersArray.add(connectedUsers[i]);
		}
		errorCode = 0;
	}
	//OTHER CMDs
	else
	{
		if(channelID == 0)	//if control channel
		{
			channelList.at(channelID)->commInterface->onCommand(doc, channelID);	//parse doc to app
			return;	//no response from here, handled by app
		}
		else
		{
			//remove later
			channelList.at(channelID)->commInterface->onCommand(doc, channelID);	//parse doc to app
		}
		/*
		else if(connectedUsers[channelID-1] == true)	//if user channel is connected
		{
			channelList.at(channelID)->commInterface->onCommand(doc, channelID);	//parse doc to app
			return; //no response from here, handled by app
		}
		else
		{
			errorCode = 3;//error response
		}
		*/
	}
	rspDoc["error"]= errorCode;					//add errorCode
	//sendResponse(rspDoc, channelID);			//send Error Response
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
	channelList[channelID]->rxCharacteristic->setValue(Output.c_str());
	channelList[channelID]->rxCharacteristic->notify(true);
}

void BluetoothService::run()
{
}
