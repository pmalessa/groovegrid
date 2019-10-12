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
	ESP_LOGI(tag,"Connected");
	connected = true;
}

void BluetoothService::onDisconnect()
{
	ESP_LOGI(tag,"Disconnected");
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
	ESP_LOGI(tag,"Hey!\n");
	ESP_LOGI(tag,"Heap: %i",xPortGetFreeHeapSize());

	debugTimer.setTimeStep(1000);

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
		ch->rxCharacteristic->setCallbacks(new CommCharacteristicReadCallback(this,ch->channelID));
		ch->txCharacteristic->setCallbacks(new CommCharacteristicWriteCallback(this,ch->channelID));
		ch->txCharacteristic->setWriteNoResponseProperty(false);
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

	xTaskCreate(runWrapper,"btTask", 2048, this,1,&btTask);
}

std::string BluetoothService::onRead(uint8_t channelID)
{
	ESP_LOGD(tag,"Read on Channel %i", channelID);
	return "0";
}

void free_msg(CommInterface::CommandMsg *msg)
{
	if(msg->doc != nullptr)delete(msg->doc);
	if(msg->rspdoc != nullptr)delete(msg->rspdoc);
	if(msg != nullptr)delete(msg);
}

void BluetoothService::onWrite(std::string data, uint8_t channelID)
{
	ESP_LOGI(tag,"OnWrite");
	if(data.empty())
	{
		return;
	}
	CommInterface::CommandMsg *msg = new CommInterface::CommandMsg;
	msg->channelID = channelID;
	msg->doc = new DynamicJsonDocument(200);
	msg->rspdoc = new DynamicJsonDocument(200);

	if(msg == nullptr)
	{
		free_msg(msg);
		return;
	}
	if(msg->doc == nullptr)
	{
		free_msg(msg);
		return;
	}
	if(msg->rspdoc == nullptr)
	{
		free_msg(msg);
		return;
	}

	ESP_LOGI(tag,"Write on Channel %i: %s",channelID,data.c_str());

	DeserializationError error = deserializeJson((*msg->doc), data);
	if (error)
	{
		ESP_LOGI(tag,"deserializeJson() failed: %s",error.c_str());
		(*msg->rspdoc)["error"]= error.c_str();				//add error
		sendResponse(msg->rspdoc, channelID);			//send Response
		free_msg(msg);
		return;										//leave if error
	}

	//----------BLE Command Parsing---------------
	(*msg->rspdoc)["rspID"] = (*msg->doc)["cmdID"];	//send cmdID back as rspID

	//CONNECT CMD
	if((*msg->doc)["cmd"] == "connect")
	{
		uint8_t userID = (*msg->doc)["userID"];
		if(userID < MAX_USERS)
		{
			if(connectedUsers[userID] != true)
			{
				connectedUsers[userID] = true; //slot is free, connect allowed
				channelList.at(userID+1)->commInterface->onCommand(msg);	//channelID= userID+1, notify game
			}
			else
			{
				(*msg->rspdoc)["error"]= 1;//send error response
			}
		}
		else
		{
			(*msg->rspdoc)["error"]= 2;//send error response
		}
	}
	//DISCONNECT CMD
	else if((*msg->doc)["cmd"] == "disconnect")
	{
		uint8_t userID = (*msg->doc)["userID"];
		if(userID < MAX_USERS)
		{
			if(connectedUsers[userID] == true)
			{
				connectedUsers[userID] = false; //slot is used, disconnect allowed
				channelList.at(userID+1)->commInterface->onCommand(msg);	//channelID= userID+1, notify game
			}
			else
			{
				(*msg->rspdoc)["error"]= 1;//send error response
			}
		}
		else
		{
			(*msg->rspdoc)["error"]= 2;//send error response
		}
	}
	//GET USER IDs CMD
	else if((*msg->doc)["cmd"] == "getUserIDs")
	{
		//send userIDs
		JsonArray usersArray = (*msg->rspdoc).createNestedArray("userIDs");
		for(uint8_t i=0;i<MAX_USERS;i++)
		{
			usersArray.add(connectedUsers[i]);
		}
		(*msg->rspdoc)["error"]= 0;
	}
	//OTHER CMDs
	else
	{
		if(channelID == 0)	//if control channel
		{
			channelList.at(channelID)->commInterface->onCommand(msg);	//parse doc to app
		}
		else
		{
			//remove later
			channelList.at(channelID)->commInterface->onCommand(msg);	//parse doc to app
		}
		/*
		else if(connectedUsers[channelID-1] == true)	//if user channel is connected
		{
			channelList.at(channelID)->commInterface->onCommand(doc, channelID);	//parse doc to app
		}
		else
		{
			errorCode = 3;//error response
		}
		*/
	}
	sendResponse(msg->rspdoc, channelID);			//send Error Response
	free_msg(msg);
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

void BluetoothService::sendResponse(DynamicJsonDocument *doc, uint8_t channelID)
{
	std::string Output;
	serializeJson(*doc, Output);
	channelList[channelID]->rxCharacteristic->setValue(Output.c_str());
	channelList[channelID]->rxCharacteristic->notify(true);
}

void BluetoothService::run()
{
	while(1)
	{
		if(debugTimer.isTimeUp())
		{
			ESP_LOGI(tag,"Heap: %i",xPortGetFreeHeapSize());
		}
		vTaskDelay(100);
	}
}
