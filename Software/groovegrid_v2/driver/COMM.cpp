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

void COMM::onRead(InputType inputType, BLECharacteristic *pCharacteristic)
{

}
void COMM::onWrite(InputType inputType, BLECharacteristic *pCharacteristic)
{
	Serial.println(inputType);

	switch (inputType) {
		case CONTROL:
			if(!controlList.empty())
			{
				for(uint16_t i=0; i<controlList.size();i++)
				{
					//controlList.at(i)->onInput((char *)pCharacteristic->getValue()[0]);
					//Serial.println((char *)pCharacteristic->getValue()[0]);
				}
			}
			break;
		case APP:
			if(!appList.empty())
			{
				for(uint16_t i=0; i<appList.size();i++)
				{
					//appList.at(i)->onInput((char *)pCharacteristic->getValue()[0]);
					//Serial.println((char *)pCharacteristic->getValue()[0]);
				}
			}
			break;
	}
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
	BluetoothService = BluetoothServer->createService(SERVICE_UUID);
	controlRxCharacteristic = BluetoothService->createCharacteristic(CONTROL_RX_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_NOTIFY);
	controlRxCharacteristic->setCallbacks(new CommCharacteristicCallback(this, CONTROL));
	controlTxCharacteristic = BluetoothService->createCharacteristic(CONTROL_TX_UUID,BLECharacteristic::PROPERTY_WRITE);
	controlTxCharacteristic->setCallbacks(new CommCharacteristicCallback(this, CONTROL));
	appRxCharacteristic = BluetoothService->createCharacteristic(APP_RX_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_NOTIFY);
	appRxCharacteristic->setCallbacks(new CommCharacteristicCallback(this, APP));
	appTxCharacteristic = BluetoothService->createCharacteristic(APP_TX_UUID,BLECharacteristic::PROPERTY_WRITE);
	appTxCharacteristic->setCallbacks(new CommCharacteristicCallback(this, APP));

	BluetoothService->start();
	BluetoothAdvertiser = BLEDevice::getAdvertising();
	BluetoothAdvertiser->addServiceUUID(SERVICE_UUID);
	BluetoothAdvertiser->setScanResponse(true);
	BluetoothAdvertiser->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	BluetoothAdvertiser->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
#endif
	controlList.setStorage(mainstorage, MAX_LISTENER_NUM, 0);
	appList.setStorage(appstorage, MAX_LISTENER_NUM, 0);
}

void COMM::Attach(InputListener *functionPointer, COMM::InputType inputType)
{
	switch (inputType) {
		case APP:
			appList.push_back(functionPointer);
			break;
		case CONTROL:
			controlList.push_back(functionPointer);
			break;
	}
}

void COMM::Detach(InputListener *functionPointer, COMM::InputType inputType)
{
	switch (inputType) {
		case APP:
			for (uint16_t i=0; i < appList.size(); i++) {
				if(appList.at(i) == functionPointer)
				{
					appList.remove(i);
				}
			}
			break;
		case CONTROL:
			for (uint16_t i=0; i < controlList.size(); i++) {
				if(controlList.at(i) == functionPointer)
				{
					controlList.remove(i);
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
}
