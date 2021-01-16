#ifndef MESSAGESERVICE_H_
#define MESSAGESERVICE_H_

#include "PLATFORM.h"
#include <string.h>
#include <vector>
#include "ArduinoJson.h"
#include "driver/CommInterface.h"
#include "driver/DeltaTimer.h"



class MessageService{
 public:
	struct ConnectedUser{
		std::string userName;
		uint32_t userAddress;
		uint16_t userID;
	};

	typedef struct{
		uint8_t channelID;
		CommInterface *commInterface;
	}CommChannel;

	static void init();
	static std::string handleMessage(std::string cmd);
	static void attachCallback(CommInterface *callbackPointer, CommInterface::ChannelID channelID);

 private:
 	static void run();

	static uint16_t connectUser(ConnectedUser user);
	static void disconnectUser(uint32_t userAddress);

	static DeltaTimer msgTaskTimer;
	static xTaskHandle msgTask;
	static std::vector<CommChannel*> channelList;
	static std::vector<ConnectedUser> connectedUserList;
	static bool isInitialized;
};

#endif /* WIFISERVICE_H_ */
