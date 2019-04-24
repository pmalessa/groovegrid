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
#if defined(__AVR__)
	Serial.begin(9600);
#elif defined(ESP32)
	Serial.begin(115200);
	Serial.print("Hey!\n");

	BLEDevice::init("GrooveGrid");
	BluetoothServer = BLEDevice::createServer();
	BluetoothServer->setCallbacks(new CommServerCallback(this));
	BluetoothAppService = BluetoothServer->createService(SERVICE_UUID);

	BluetoothAppService->start();
	BluetoothAdvertiser = BLEDevice::getAdvertising();
	BluetoothAdvertiser->addServiceUUID(SERVICE_UUID);
	BluetoothAdvertiser->setScanResponse(true);
	BluetoothAdvertiser->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	BluetoothAdvertiser->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
#endif
	callbackList.setStorage(callbackstorage, MAX_CALLBACK_NUM, 0);
}

void COMM::Attach(CommListener *callbackPointer)
{
	controlRxCharacteristic = BluetoothAppService->createCharacteristic(CONTROL_RX_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_NOTIFY);
	controlRxCharacteristic->setCallbacks(new CommListener(this, CONTROL));
	callbackList.push_back(callbackPointer);
}

void COMM::Detach(CommListener *callbackPointer)
{
	for (uint16_t i=0; i < appList.size(); i++) {
		if(appList.at(i) == functionPointer)
		{
			appList.remove(i);
		}
	}
}

void COMM::run()
{
	/*
	if(BUTTON_bIsPressed(BUTTON_UP))
		//send u
	if(BUTTON_bIsPressed(BUTTON_DOWN))
		//send d
	if(BUTTON_bIsPressed(BUTTON_LEFT))
		//send l
	if(BUTTON_bIsPressed(BUTTON_RIGHT))
		//send r
	 */
}
