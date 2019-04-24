/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef COMM_H_
#define COMM_H_

/*Todo:
 * restructure the COMM Class:
 *  - The BLE Communication is split into separate services, one control service, one game service and maybe multiple user input services.
 *  - Each App/Game implements a function to return a structure containing multiple keywords, combined with function pointers and Read/Write property
 *  which will then be passed to COMM to create characteristics inside this service. This way the Apps have no direct connection to the hardware,
 *  they just expect those functions to be called once a read or write request is issued by the mobile app
 *
 */

#include "../PLATFORM.h"
#include "../utils/Task.h"
#include "../utils/Vector.h"
#include "../utils/InputListener.h"

#define MAX_LISTENER_NUM 10
#if defined(__AVR__)
#include "HardwareSerial.h"
#elif defined(ESP32)
#include "BLE/BLEDevice.h"
#endif

class COMM : public Task{

//randomly generated
#define SERVICE_UUID        "66c93897-a5f9-4a03-9d77-de1404d39270"	// See the following for generating UUIDs:
#define CONTROL_RX_UUID 	"9e21d8fd-8837-482f-93ac-d9d81db00f33"	// https://www.uuidgenerator.net/
#define CONTROL_TX_UUID 	"9e21d8fd-8837-482f-93ac-d9d81db00f34"
#define APP_RX_UUID 		"9e21d8fd-8837-482f-93ac-d9d81db00f35"
#define APP_TX_UUID 		"9e21d8fd-8837-482f-93ac-d9d81db00f36"

 public:
	enum InputType{
		CONTROL,
		APP
	};

	static COMM& getInstance();
	virtual ~COMM(void);
	void         Attach(InputListener *obj, COMM::InputType inputType);
	void         Detach(InputListener *obj, COMM::InputType inputType);
	bool isConnected();
	void run();

 private:
	COMM();
	COMM(const COMM&);
	COMM & operator = (const COMM &);
#ifdef ESP32
	void onRead(InputType inputType, BLECharacteristic *pCharacteristic);
	void onWrite(InputType inputType, BLECharacteristic *pCharacteristic);
	void onConnect();
	void onDisconnect();

	bool connected = false;
	BLEServer *BluetoothServer;
	BLEService *BluetoothService;
	BLECharacteristic *controlRxCharacteristic, *controlTxCharacteristic, *appRxCharacteristic, *appTxCharacteristic;
	BLEAdvertising *BluetoothAdvertiser;

	class CommCharacteristicCallback : public BLECharacteristicCallbacks
	{
	public:
		CommCharacteristicCallback(COMM *commPtr, InputType inputType)
		{
			this->commPtr = commPtr;
			this->inputType = inputType;
		}
	    void onRead(BLECharacteristic *pCharacteristic){	commPtr->onRead(inputType, pCharacteristic);}	//pass call to COMM
	    void onWrite(BLECharacteristic *pCharacteristic){	commPtr->onWrite(inputType, pCharacteristic);}	//pass call to COMM
	private:
	    COMM *commPtr;
	    InputType inputType;
	};
	class CommServerCallback : public BLEServerCallbacks
	{
	public:
		CommServerCallback(COMM *commPtr){		this->commPtr = commPtr;}
		void onConnect(BLEServer* pServer){		commPtr->onConnect();}			//pass call to COMM
		void onDisconnect(BLEServer* pServer){	commPtr->onDisconnect();}		//pass call to COMM
	private:
		COMM *commPtr;
	};
#endif
    Vector<InputListener*> controlList;
    InputListener* mainstorage[MAX_LISTENER_NUM];
    Vector<InputListener*> appList;
    InputListener* appstorage[MAX_LISTENER_NUM];

};


#endif /* COMM_H_ */
