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
 *  they just expect those functions to be called once a read or write request is issued by the mobile app.
 *
 *  Move the registration of callback functions based on keywords into each game, easier than passing a structure.
 *  Once the game gets destroyed, the service and therefore the created characteristics get destroyed as well. Therefore no loss of control.
 *
 */

#include "../PLATFORM.h"
#include "../utils/Task.h"
#include "../utils/Vector.h"
#include "../utils/InputListener.h"

#define MAX_CALLBACK_NUM 10
#if defined(__AVR__)
#include "HardwareSerial.h"
#elif defined(ESP32)
#include "BLE/BLEDevice.h"
#endif

class CommListener : public BLECharacteristicCallbacks
{
	enum InputType{
		CONTROL,
		APP
	};
public:
	CommListener(BLEUUID uuid, std::string keyword, InputType inputType)
	{
		this->uuid = uuid;
		this->keyword = keyword;
		this->inputType = inputType;
	}
    virtual std::string onRead() = 0;
    virtual void onWrite(std::string data) = 0;
private:
    BLEUUID uuid;
    std::string keyword;
    InputType inputType;
};

/*
 * This CommCallback class gets attached to the specific characteristic.
 * it calls the onRead and onWrite functions of its commListener and passes the values along.
 */
class CommCallback : public BLECharacteristicCallbacks
{
public:
	CommCallback(CommListener *commListener){this->commListener = commListener;};
    void onRead(BLECharacteristic* pCharacteristic)
    {
    	std::string res = commListener->onRead();
    	pCharacteristic->setValue(res);
    }
    void onWrite(BLECharacteristic* pCharacteristic)
    {
    	std::string res = pCharacteristic->getValue();
    	commListener->onWrite(res);
    }
private:
    CommListener *commListener;
};

class COMM : public Task{

//randomly generated
#define SERVICE_UUID        "66c93897-a5f9-4a03-9d77-de1404d39270"	// See the following for generating UUIDs:
#define CONTROL_RX_UUID 	"9e21d8fd-8837-482f-93ac-d9d81db00f33"	// https://www.uuidgenerator.net/
#define CONTROL_TX_UUID 	"9e21d8fd-8837-482f-93ac-d9d81db00f34"
#define APP_RX_UUID 		"9e21d8fd-8837-482f-93ac-d9d81db00f35"
#define APP_TX_UUID 		"9e21d8fd-8837-482f-93ac-d9d81db00f36"

 public:
	static COMM& getInstance();
	virtual ~COMM(void);
	void Attach(CommListener *callback);
	void Detach(CommListener *callback);
	bool isConnected();
	void run();

 private:
	COMM();
	COMM(const COMM&);
	COMM& operator = (const COMM&);
#ifdef ESP32
	void onConnect();
	void onDisconnect();

	bool connected = false;
	BLEServer *BluetoothServer;
	BLEService *BluetoothAppService, *BluetoothGameService;
	BLEAdvertising *BluetoothAdvertiser;

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
	//Todo: replace with standard c++ vector implementation once AVR is eliminated
    Vector<CommListener*> callbackList;
    CommListener* callbackstorage[MAX_CALLBACK_NUM];

};

#endif /* COMM_H_ */
