/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef BLUETOOTHSERVICE_H_
#define BLUETOOTHSERVICE_H_


#include "../PLATFORM.h"
#include "ArduinoJson.h"
#include "CommInterface.h"
#include "DeltaTimer.h"

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
	BLE2902 *rxdescriptor, *txdescriptor;
}CommChannel;

struct ConnectedDevice{
	std::string deviceName;
	std::string deviceAddress;
	uint16_t connectionID;
	ConnectedDevice(std::string a, uint16_t b){
		deviceName = ""; deviceAddress = a; connectionID = b;};
};

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


class BluetoothService{

//randomly generated
// https://www.uuidgenerator.net/
#define SERVICE_CONTROL_UUID        "66c93897-a5f9-4a03-9d77-de1404d39270"
#define SERVICE_USER1_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39271"
#define SERVICE_USER2_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39272"
#define SERVICE_USER3_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39273"
#define SERVICE_USER4_UUID        	"66c93897-a5f9-4a03-9d77-de1404d39274"
#define RX_UUID 			        "9e21d8fd-8837-482f-93ac-d9d81db00f33"
#define TX_UUID 			        "9e21d8fd-8837-482f-93ac-d9d81db00f34"

 public:
	static BluetoothService& getInstance();
	virtual ~BluetoothService(void);
	void sendResponse(DynamicJsonDocument *doc, uint8_t channelID);
	bool isConnected();
	void Attach(CommInterface *callback, ChannelID channelID);
	void run();
	
 private:
	BluetoothService();
	BluetoothService(const BluetoothService&);
	BluetoothService& operator = (const BluetoothService&);
	void onConnect(esp_ble_gatts_cb_param_t *param);
	void onDisconnect(esp_ble_gatts_cb_param_t *param);
	std::string onRead(uint8_t channelID, uint16_t conn_id);
	void onWrite(std::string data, uint8_t channelID, uint16_t conn_id);

	bool connected = false;
	BLEServer *BluetoothServer;
	BLEAdvertising *BluetoothAdvertiser;

	const char* tag = "btService";
	DeltaTimer btTaskTimer;
	xTaskHandle btTask;

	static void runWrapper(void* _this){((BluetoothService*)_this)->run();}

	class CommServerCallback : public BLEServerCallbacks
	{
	public:
		CommServerCallback(BluetoothService *commPtr){		this->commPtr = commPtr;}
		void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){		commPtr->onConnect(param);}			//pass call to COMM
		void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){	commPtr->onDisconnect(param);}		//pass call to COMM
	private:
		BluetoothService *commPtr;
	};

	class CommCharacteristicReadCallback : public BLECharacteristicCallbacks
	{
	public:
		CommCharacteristicReadCallback(BluetoothService *commPtr, uint8_t channelID){this->commPtr = commPtr; this->channelID = channelID;};
	    void onRead(BLECharacteristic* pCharacteristic, uint16_t conn_id)
	    {
	    	pCharacteristic->setValue(commPtr->onRead(channelID, conn_id));
	    }
	private:
	    BluetoothService *commPtr;
	    uint8_t channelID;
	};
	class CommCharacteristicWriteCallback : public BLECharacteristicCallbacks
	{
	public:
		CommCharacteristicWriteCallback(BluetoothService *commPtr, uint8_t channelID){this->commPtr = commPtr; this->channelID = channelID;};
	    void onWrite(BLECharacteristic* pCharacteristic, uint16_t conn_id)
	    {
	    	commPtr->onWrite(pCharacteristic->getValue(), channelID, conn_id);//call COMM passUp function here
	    }
	private:
	    BluetoothService *commPtr;
	    uint8_t channelID;
	};

    std::vector<CommChannel*> channelList;
	std::vector<ConnectedDevice> connectedDeviceList;
};

#endif /* BLUETOOTHSERVICE_H_ */
