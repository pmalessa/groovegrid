#ifndef MESSAGESERVICE_H_
#define MESSAGESERVICE_H_

#include "PLATFORM.h"
#include <string.h>
#include <vector>
#include "ArduinoJson.h"
#include "driver/CommInterface.h"
#include "driver/DeltaTimer.h"

struct ConnectedUser{
	std::string userName;
	uint32_t userAddress;
	uint16_t userID;
};

typedef struct{
	uint8_t channelID;
    CommInterface *commInterface;
}CommChannel;

class MessageService{

 public:
	static MessageService& getInstance();
	virtual ~MessageService(void);
	void run();
	std::string handleMessage(std::string cmd);
	void attachCallback(CommInterface *callbackPointer, ChannelID channelID);

 private:
	MessageService();
	MessageService(const MessageService&);
	MessageService& operator = (const MessageService&);

	uint16_t connectUser(ConnectedUser user);
	void disconnectUser(uint32_t userAddress);

	static void runWrapper(void* _this){((MessageService*)_this)->run();}

	DeltaTimer msgTaskTimer;
	xTaskHandle msgTask;

	std::vector<CommChannel*> channelList;
	std::vector<ConnectedUser> connectedUserList;
};

#endif /* WIFISERVICE_H_ */
