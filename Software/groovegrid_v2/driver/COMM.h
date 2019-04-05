/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef COMM_H_
#define COMM_H_

#include "../PLATFORM.h"

#if defined(__AVR__)
#include "HardwareSerial.h"
#elif defined(ESP32)
#include "BLE/BLEDevice.h"
#endif

class COMM {

//randomly generated
#define SERVICE_UUID        "66c93897-a5f9-4a03-9d77-de1404d39270"	// See the following for generating UUIDs:
#define CHARACTERISTIC_UUID "9e21d8fd-8837-482f-93ac-d9d81db00f33"	// https://www.uuidgenerator.net/

 public:
	enum Event{
		SWIPEDIRECTION,
		GAMESTATE,
		SERIALINPUT
	};

	static COMM& getInstance();
	~COMM(void);
	int 		 read();
	void         setCallback(void *functionPointer, COMM::Event eventType);
	void         removeCallback(void *functionPointer);

 private:
	COMM();
	COMM(const COMM&);
	COMM & operator = (const COMM &);
#ifdef ESP32
	BLEServer *BluetoothServer;
	BLEService *BluetoothService;
	BLECharacteristic *BluetoothCharacteristic;
	BLEAdvertising *BluetoothAdvertiser;
#endif
};


#endif /* COMM_H_ */
