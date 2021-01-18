#ifndef MESSAGESERVICE_H_
#define MESSAGESERVICE_H_

#include "PLATFORM.h"
#include "ArduinoJson.h"

class MessageService{
 public:
	typedef struct {
		StaticJsonDocument<512> doc;
		StaticJsonDocument<512> rspdoc;
		uint8_t channelID;
	}CommandMsg;

	typedef std::function<void(CommandMsg&)> MessageCallback;
	typedef uint8_t CallbackID;

	static void init();
	static std::string handleMessage(std::string cmd);
	static CallbackID attachCallback(MessageCallback callback);
	static bool removeCallback(CallbackID id);

 private:
 	static void run();

	typedef struct {
		MessageCallback function;
		CallbackID id;
	}callbackListEntry;

	static DeltaTimer msgTaskTimer;
	static xTaskHandle msgTask;
	static std::vector<callbackListEntry> callbackList;
	static bool isInitialized;
};

#endif /* MESSAGESERVICE_H_ */
