/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "COMM.h"
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
	BLEDevice::init("GrooveGrid");
	BluetoothServer = BLEDevice::createServer();
	BluetoothService = BluetoothServer->createService(SERVICE_UUID);
	BluetoothCharacteristic = BluetoothService->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_WRITE);
	BluetoothCharacteristic->setValue("Let´s Groove!");
	BluetoothService->start();
	BluetoothAdvertiser = BLEDevice::getAdvertising();
	BluetoothAdvertiser->addServiceUUID(SERVICE_UUID);
	BluetoothAdvertiser->setScanResponse(true);
	BluetoothAdvertiser->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	BluetoothAdvertiser->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
#endif
	mainlist.setStorage(mainstorage, MAX_LISTENER_NUM, 0);
	applist.setStorage(appstorage, MAX_LISTENER_NUM, 0);
}

void COMM::Attach(InputListener *functionPointer, COMM::InputType inputType)
{
	switch (inputType) {
		case APP:
			applist.push_back(functionPointer);
			break;
		case MAIN:
			mainlist.push_back(functionPointer);
			break;
	}
}

void COMM::Detach(InputListener *functionPointer, COMM::InputType inputType)
{
	switch (inputType) {
		case APP:
			for (uint16_t i=0; i < applist.size(); i++) {
				if(applist.at(i) == functionPointer)
				{
					applist.remove(i);
				}
			}
			break;
		case MAIN:
			for (uint16_t i=0; i < mainlist.size(); i++) {
				if(mainlist.at(i) == functionPointer)
				{
					mainlist.remove(i);
				}
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
	char byte = Serial.read();
	switch (byte) {
		case -1:
			break;
		case '1':
		case 'q':
		case 'x':
			main_send(byte);	//change to main
			break;
		default:
			app_send(byte);
			break;
	}
}

void COMM::main_send(char byte)
{
	if(!mainlist.empty())
	{
		for(uint16_t i=0; i<mainlist.size();i++)
		{
			mainlist.at(i)->onInput(&byte);
		}
	}
}

void COMM::app_send(char byte)
{
	if(!applist.empty())
	{
		for(uint16_t i=0; i<applist.size();i++)
		{
			applist.at(i)->onInput(&byte);
		}
	}
}
