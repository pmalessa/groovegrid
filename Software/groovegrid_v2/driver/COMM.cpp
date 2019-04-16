/*
 * COMM.cpp
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */
#include "COMM.h"
uint8_t deviceConnected;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class FromGridCallback: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic) {
    }
};

class ToGridCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
    	static COMM& comm = COMM::getInstance();
    	std::string rxValue = pCharacteristic->getValue();
    	switch (rxValue[0]) {
    		case -1:
    			break;
    		case '1':
    		case '2':
    		case 'q':
    		case 'x':
    			comm.main_send(rxValue[0]);	//change to main
    			break;
    		default:
    			comm.app_send(rxValue[0]);
    			break;
    	}
    }
};

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
	BLEDevice::init("GrooveGrid");
	BluetoothServer = BLEDevice::createServer();
	BluetoothServer->setCallbacks(new MyServerCallbacks());
	BluetoothService = BluetoothServer->createService(SERVICE_UUID);
	fromGridCharacteristic = BluetoothService->createCharacteristic(CHAR_FROMGRID_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_NOTIFY);
	fromGridCharacteristic->setValue("Read Data here");
	fromGridCharacteristic->setCallbacks(new FromGridCallback());
	toGridCharacteristic = BluetoothService->createCharacteristic(CHAR_TOGRID_UUID,BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
	toGridCharacteristic->setValue("Write Data here");
	toGridCharacteristic->setCallbacks(new ToGridCallback());
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
	int byte = Serial.read();
	switch (byte) {
		case -1:
			break;
		case '1':
		case '2':
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
