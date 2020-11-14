
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

//add User to connectedUserList and return userID.
uint16_t MessageService::connectUser(ConnectedUser user)
{
	uint8_t userBitmap = 0;
	ESP_LOGI(TAG,"User Connected, name: %s, address: 0x%X",user.userName.c_str(), user.userAddress);
	for(uint8_t i = 0; i< connectedUserList.size();i++)	//check if user is in connectedUserList already
	{
		if(connectedUserList.at(i).userAddress == user.userAddress)
		{
			ESP_LOGI(TAG,"Already connected, userID: %i",connectedUserList.at(i).userID);
			return connectedUserList.at(i).userID;
		}
		userBitmap |= (1<<connectedUserList.at(i).userID);	//create Bitmap of used userIDs
	}
	for(uint8_t i=1;i<=4;i++)	//check which userID is free, 1..4
	{
		if(!(userBitmap & (1<<i)))
		{
			user.userID = i;
			connectedUserList.push_back(user);
			ESP_LOGI(TAG,"assigned userID: %i",user.userID);
			return user.userID;
		}
	}
	return 0;
}

//remove User from connectedUserList
void MessageService::disconnectUser(uint32_t userAddress)
{
	ESP_LOGI(TAG,"User Disconnected, address: 0x%X",userAddress);
	for(uint8_t i = 0; i< connectedUserList.size();i++)
	{
		if(connectedUserList.at(i).userAddress == userAddress)
		{
			ESP_LOGI(TAG,"Erased element nr %i",i);
			connectedUserList.erase(connectedUserList.begin()+i);
			return;
		}
	}
	ESP_LOGI(TAG,"User not connected, address: 0x%X", userAddress);
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
		//TODO: include address and name
		(*msg->rspdoc)["error"]= 0;
	}
	//CONNECT USER
	else if((*msg->doc)["cmd"] == "connectUser")
	{
		//check user address in connectedUserList
		//return userID if it exists, else return new number and connect new user
		ConnectedUser user;
		//user.userName = (*msg->doc)["userName"];
		user.userAddress = (*msg->doc)["uuid"];
		user.userID = connectUser(user);
		if(user.userID == 0)	//if no free slot
		{
			(*msg->rspdoc)["error"]= 2;
		}
		else
		{
			(*msg->rspdoc)["userID"]=  user.userID;
			(*msg->rspdoc)["error"]= 0;
		}
	}
	//DISCONNECT USER
	else if((*msg->doc)["cmd"] == "disconnectUser")
	{
		disconnectUser((*msg->doc)["uuid"]);
		(*msg->rspdoc)["error"]= 0;
	}
	//OTHER CMDs
	else
	{
		uint8_t userID = (*msg->doc)["userID"];
		if(msg->doc->containsKey("userID"))
		{
			channelList.at(userID)->commInterface->onCommand(msg);	//parse doc to user channel
		}
		else
		{
			channelList.at(0)->commInterface->onCommand(msg);	//parse doc to control channel
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
				ESP_LOGI(TAG,"userID: %i \t address: 0x%X",connectedUserList.at(i).userID,connectedUserList.at(i).userAddress);

			}
		}
		vTaskDelay(100);
	}
}