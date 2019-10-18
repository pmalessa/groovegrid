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

void BluetoothService::onConnect(esp_ble_gatts_cb_param_t *param)
{
	char buf[20];
	uint8_t* i = param->connect.remote_bda;
	sprintf(buf,"%X:%X:%X:%X:%X:%X",i[0],i[1],i[2],i[3],i[4],i[5]);
	std::string str(buf);
	ESP_LOGI(tag,"Connected, conn_id %i, address: %s",param->connect.conn_id, str.c_str());

	ConnectedDevice dev(str,param->connect.conn_id);
	connectedDeviceList.push_back(dev);
	connected = true;
	BLEDevice::startAdvertising();
}

void BluetoothService::onDisconnect(esp_ble_gatts_cb_param_t *param)
{
	char buf[20];
	uint8_t* i = param->disconnect.remote_bda;
	sprintf(buf,"%X:%X:%X:%X:%X:%X",i[0],i[1],i[2],i[3],i[4],i[5]);
	std::string str(buf);
	ESP_LOGI(tag,"Disconnected, conn_id %i, address: %s",param->disconnect.conn_id,str.c_str());
	connected = false;
	for(uint8_t i = 0; i< connectedDeviceList.size();i++)
	{
		if(connectedDeviceList.at(i).connectionID == param->disconnect.conn_id)
		{
			ESP_LOGI(tag,"Erased element nr %i",i);
			connectedDeviceList.erase(connectedDeviceList.begin()+i);
			return;
		}
	}
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
	btTaskTimer.setTimeStep(3000);

	/*SERVER*/
	BLEDevice::init("GrooveGrid");
	BLEDevice::setMTU(185);
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
	BLEDevice::startAdvertising();

	xTaskCreatePinnedToCore(runWrapper,"btTask", 2048, this,1,&btTask,0);
}

std::string BluetoothService::onRead(uint8_t channelID, uint16_t conn_id)
{
	ESP_LOGI(tag,"Read on Channel %i, conn_id %i", channelID, conn_id);
	return "0";
}

void free_msg(CommInterface::CommandMsg *msg)
{
	if(msg->doc != nullptr)delete(msg->doc);
	if(msg->rspdoc != nullptr)delete(msg->rspdoc);
	if(msg != nullptr)delete(msg);
}

void BluetoothService::onWrite(std::string data, uint8_t channelID, uint16_t conn_id)
{
	ESP_LOGI(tag,"Write on Channel %i, conn_id %i", channelID, conn_id);
	if(channelID > 0 && channelID != (conn_id+1))
	{
		ESP_LOGI(tag,"wrong channel");
		return;//each user is only allowed to write on the control channel or its user channel
	}
	if(data.empty())
	{
		ESP_LOGI(tag,"empty");
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

	ESP_LOGI(tag,"Message: %s",data.c_str());

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

	//GET CONNECTED USERS
	if((*msg->doc)["cmd"] == "getConnectedUsers")
	{
		//send connectionIDs
		JsonArray usersArray = (*msg->rspdoc).createNestedArray("userIDs");
		for(uint8_t i=0;i<connectedDeviceList.size();i++)
		{
			usersArray.add(connectedDeviceList.at(i).connectionID);
		}
		(*msg->rspdoc)["error"]= 0;
	}
	//GET USER ID
	if((*msg->doc)["cmd"] == "getUserID")
	{
		//send connectionID
		(*msg->rspdoc)["userID"]= conn_id;
		(*msg->rspdoc)["error"]= 0;
	}
	//OTHER CMDs
	else
	{
		if(channelID == 0)	//if control channel
		{
			channelList.at(channelID)->commInterface->onCommand(msg);	//parse doc to app
		}
		else //send to user channel, conn_id 0..3 -> user_id 1..4
		{
			channelList.at(conn_id + 1)->commInterface->onCommand(msg);	//parse doc to app
		}
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
		if(btTaskTimer.isTimeUp())
		{
			ESP_LOGI(tag,"Connected devices: %i",connectedDeviceList.size());
			for(uint8_t i = 0; i< connectedDeviceList.size();i++)
			{
				std::string str = connectedDeviceList.at(i).deviceAddress;
				ESP_LOGI(tag,"conn_id %i \t address: %s",connectedDeviceList.at(i).connectionID,str.c_str());

			}
		}
		vTaskDelay(100);
	}
}
