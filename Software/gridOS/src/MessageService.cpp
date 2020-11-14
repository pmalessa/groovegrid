
#include "MessageService.h"

static const char* TAG = "msgService";

MessageService::MessageService()
{
	CommChannel *ch;
	msgTaskTimer.setTimeStep(3000);
	xTaskCreatePinnedToCore(runWrapper,"msgTask", 2048, this,1,&msgTask,0);

	ch = new CommChannel;
	ch->channelID = CHANNEL_CONTROL;
	channelList.push_back(ch);
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER1;
	channelList.push_back(ch);
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER2;
	channelList.push_back(ch);
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER3;
	channelList.push_back(ch);
	ch = new CommChannel;
	ch->channelID = CHANNEL_USER4;
	channelList.push_back(ch);
}
MessageService::~MessageService(){}

MessageService& MessageService::getInstance()
{
	static MessageService _instance;
	return _instance;
}

void MessageService::connectUser(ConnectedUser user)
{
	ESP_LOGI(TAG,"User Connected, name: %s, address: %s, id: %i",user.userName.c_str(), user.userAddress.c_str(), user.userID);

	connectedUserList.push_back(user);
}

void MessageService::disconnectUser(ConnectedUser user)
{

	ESP_LOGI(TAG,"User Disconnected, name: %s, address: %s, id: %i",user.userName.c_str(), user.userAddress.c_str(), user.userID);
	for(uint8_t i = 0; i< connectedUserList.size();i++)
	{
		if(connectedUserList.at(i).userID == user.userID)
		{
			ESP_LOGI(TAG,"Erased element nr %i",i);
			connectedUserList.erase(connectedUserList.begin()+i);
			return;
		}
	}
}

void free_msg(CommInterface::CommandMsg *msg)
{
	if(msg->doc != nullptr)delete(msg->doc);
	if(msg->rspdoc != nullptr)delete(msg->rspdoc);
	if(msg != nullptr)delete(msg);
}

std::string MessageService::handleMessage(std::string cmd)
{
	CommInterface::CommandMsg *msg = new CommInterface::CommandMsg;
	msg->doc = new DynamicJsonDocument(500);
	msg->rspdoc = new DynamicJsonDocument(500);
	std::string output;

	if(msg == nullptr)
	{
		free_msg(msg);
		return "";
	}
	if(msg->doc == nullptr)
	{
		free_msg(msg);
		return "";
	}
	if(msg->rspdoc == nullptr)
	{
		free_msg(msg);
		return "";
	}

	ESP_LOGI(TAG,"Message: %s",cmd.c_str());
    DeserializationError error = deserializeJson((*msg->doc), cmd);
	if (error)
	{
		ESP_LOGI(TAG,"deserializeJson() failed: %s",error.c_str());
		(*msg->rspdoc)["error"]= error.c_str();				//add error
		serializeJson((*msg->rspdoc),output);
		free_msg(msg);
		return output;									//leave if error
	}

	//----------Command Parsing---------------
	(*msg->rspdoc)["rspID"] = (*msg->doc)["cmdID"];	//send cmdID back as rspID

	//GET CONNECTED USERS
	if((*msg->doc)["cmd"] == "getConnectedUsers")
	{
		//send connectionIDs
		JsonArray usersArray = (*msg->rspdoc).createNestedArray("userIDs");
		for(uint8_t i=0;i<connectedUserList.size();i++)
		{
			usersArray.add(connectedUserList.at(i).userID + 1);	//1..4
		}
		(*msg->rspdoc)["error"]= 0;
	}
	//GET USER ID
	if((*msg->doc)["cmd"] == "getUserID")
	{
		//check user address in connectedUserList
		//return userID if it exists, else return new number and connect new user
		(*msg->rspdoc)["userID"]=  1;	//1..4 TODO: fix
		(*msg->rspdoc)["error"]= 0;
	}
	//OTHER CMDs
	else
	{
		uint8_t userID = (*msg->doc)["userID"];
		if(msg->doc->containsKey("userID"))
		{
			channelList.at(userID)->commInterface->onCommand(msg);	//parse doc to app
		}
		else
		{
			channelList.at(0)->commInterface->onCommand(msg);	//parse doc to app
		}
	}
	serializeJson((*msg->rspdoc), output);
	free_msg(msg);
	return output;
}

void MessageService::attachCallback(CommInterface *callbackPointer, ChannelID channelID)
{
    for (uint16_t i=0; i < channelList.size(); i++) {
        if(channelList.at(i)->channelID == channelID)
        {
            channelList.at(i)->commInterface = callbackPointer;
			channelList.at(i)->channelID = channelID;
        }
    }
}

void MessageService::run()
{
	while(1)
	{
		if(msgTaskTimer.isTimeUp())
		{
			ESP_LOGI(TAG,"Connected devices: %i",connectedUserList.size());
			for(uint8_t i = 0; i< connectedUserList.size();i++)
			{
				std::string str = connectedUserList.at(i).userAddress;
				ESP_LOGI(TAG,"conn_id %i \t address: %s",connectedUserList.at(i).userID,str.c_str());

			}
		}
		vTaskDelay(100);
	}
}