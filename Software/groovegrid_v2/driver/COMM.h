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
#include <vector>
#include "CommInterface.h"

#define MAX_CHANNEL_NUM 10
#include "BLEDevice.h"
#include "BLE2902.h"

typedef struct{
	std::string channelName;
	uint8_t channelID;
	BLEUUID *serviceUUID, *rxUUID, *txUUID;
	CommInterface *commInterface;
	BLECharacteristic *rxCharacteristic, *txCharacteristic;
	BLEService *attachedService;
}CommChannel;

enum ChannelID{
	CHANNEL_CONTROL,
	CHANNEL_USER1,
	CHANNEL_USER2,
	CHANNEL_USER3,
	CHANNEL_USER4
};

/*
 * This CommCallback class gets attached to the specific characteristic.
 * it passes the values up.
 */


class COMM : public Task{

//randomly generated
// https://www.uuidgenerator.net/
#define SERVICE_CONTROL_UUID        "66c93897-a5f9-4a03-9d77-de1404d39270"
#define SERVICE_USER1_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39271"
#define SERVICE_USER2_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39272"
#define RX_UUID 			        "9e21d8fd-8837-482f-93ac-d9d81db00f33"
#define TX_UUID 			        "9e21d8fd-8837-482f-93ac-d9d81db00f34"

 public:
	static COMM& getInstance();
	virtual ~COMM(void);
	void Attach(CommInterface *callback, ChannelID channel);
	void Detach(CommInterface *callback);
	bool isConnected();
	void run();

 private:
	COMM();
	COMM(const COMM&);
	COMM& operator = (const COMM&);
	void onConnect();
	void onDisconnect();
	std::string onRead(uint8_t channelID);
	void onWrite(std::string data, uint8_t channelID);

	bool connected = false;
	BLEServer *BluetoothServer;
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
	class CommCharacteristicCallback : public BLECharacteristicCallbacks
	{
	public:
		CommCharacteristicCallback(COMM *commPtr, uint8_t channelID){this->commPtr = commPtr; this->channelID = channelID;};
	    void onRead(BLECharacteristic* pCharacteristic)
	    {
	    	std::string res = commPtr->onRead(channelID); //call COMM passUp function here
	    	pCharacteristic->setValue(res);
	    }
	    void onWrite(BLECharacteristic* pCharacteristic)
	    {
	    	std::string res = pCharacteristic->getValue();
	    	commPtr->onWrite(res, channelID);//call COMM passUp function here
	    }
	private:
	    COMM *commPtr;
	    uint8_t channelID;
	};

    std::vector<CommChannel*> channelList;

};

#endif /* COMM_H_ */
