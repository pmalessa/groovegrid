/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef COMM_H_
#define COMM_H_

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
#define CHAR_FROMGRID_UUID "9e21d8fd-8837-482f-93ac-d9d81db00f33"	// https://www.uuidgenerator.net/
#define CHAR_TOGRID_UUID "9e21d8fd-8837-482f-93ac-d9d81db00f34"

 public:
	enum InputType{
		MAIN,
		APP
	};

	static COMM& getInstance();
	virtual ~COMM(void);
	void         Attach(InputListener *obj, COMM::InputType inputType);
	void         Detach(InputListener *obj, COMM::InputType inputType);
	void run();
	void app_send(char byte);
	void main_send(char byte);

 private:
	COMM();
	COMM(const COMM&);
	COMM & operator = (const COMM &);
#ifdef ESP32
	BLEServer *BluetoothServer;
	BLEService *BluetoothService;
	BLECharacteristic *fromGridCharacteristic, *toGridCharacteristic;
	BLEAdvertising *BluetoothAdvertiser;
#endif



    Vector<InputListener*> mainlist;
    InputListener* mainstorage[MAX_LISTENER_NUM];
    Vector<InputListener*> applist;
    InputListener* appstorage[MAX_LISTENER_NUM];
};


#endif /* COMM_H_ */
