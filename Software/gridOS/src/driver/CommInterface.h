/*
 * CommInterface.h
 *
 *  Created on: 25.04.2019
 *      Author: pmale
 */

#ifndef DRIVER_COMMINTERFACE_H_
#define DRIVER_COMMINTERFACE_H_

#include "../PLATFORM.h"
#include "ArduinoJson.h"

class CommInterface
{
public:
	enum ChannelID{
		CHANNEL_CONTROL,
		CHANNEL_USER1,
		CHANNEL_USER2,
		CHANNEL_USER3,
		CHANNEL_USER4
	};
	
	typedef struct{
		DynamicJsonDocument *doc;
		DynamicJsonDocument *rspdoc;
		uint8_t channelID;
	}CommandMsg;
	virtual ~CommInterface(){};
    virtual void onCommand(CommandMsg *msg) = 0;
private:
};




#endif /* DRIVER_COMMINTERFACE_H_ */
