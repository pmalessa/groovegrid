
#include "MessageService.h"

static const char* TAG = "msgService";
DeltaTimer MessageService::msgTaskTimer;
xTaskHandle MessageService::msgTask;
bool MessageService::isInitialized = false;
std::vector<MessageService::callbackListEntry> MessageService::callbackList;

void MessageService::init()
{
	if(!isInitialized){
		isInitialized = true;
		msgTaskTimer.setTimeStep(3000);
		xTaskCreatePinnedToCore((TaskFunction_t)run,"messageTask", 2048, nullptr,1,&msgTask,0);
	}
}

std::string MessageService::handleMessage(std::string cmd)
{
	if(!isInitialized)init();
	CommandMsg msg;
	std::string output;

	ESP_LOGI(TAG,"Message: %s",cmd.c_str());
    DeserializationError error = deserializeJson((msg.doc), cmd);
	if (error)
	{
		ESP_LOGI(TAG,"deserializeJson() failed: %s",error.c_str());
		(msg.rspdoc)["error"]= error.c_str();				//add error
	}
	else
	{
		msg.rspdoc["rspID"] = msg.doc["cmdID"];	//send cmdID back as rspID
		for(uint8_t i=0; i< callbackList.size();i++)	//call all listeners
		{
			callbackList.at(i).function(msg);
		}
	}
	serializeJson((msg.rspdoc), output);
	return output;
}

MessageService::CallbackID MessageService::attachCallback(MessageService::MessageCallback callback)
{
	callbackListEntry entry;
	if(!isInitialized)init();
	if(callback != nullptr)
	{
		entry.function = callback;
		bool duplicate = false;
		do{
			duplicate = false;
			entry.id = rand()%0xFF;	//generate random id
			for(uint8_t i=0;i<callbackList.size();i++)
			{
				if(callbackList.at(i).id == entry.id)
				{	//if duplicate id, retry
					duplicate = true;
				}
			}
		}while(duplicate == true);
		callbackList.push_back(entry);	//add to callbackList
	}
	return entry.id;	//ID of the callback
}

bool MessageService::removeCallback(MessageService::CallbackID id)
{
	if(!isInitialized)init();
	for(uint8_t i=0;i<callbackList.size();i++)
	{
		if(callbackList.at(i).id == id){
			callbackList.erase(callbackList.begin()+i);
			return true;
		}
	}
	return false;
}

void MessageService::run()
{
	while(1)
	{
		if(msgTaskTimer.isTimeUp())
		{
		}
		vTaskDelay(100);
	}
}