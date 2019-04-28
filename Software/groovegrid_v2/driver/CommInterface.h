/*
 * CommInterface.h
 *
 *  Created on: 25.04.2019
 *      Author: pmale
 */

#ifndef DRIVER_COMMINTERFACE_H_
#define DRIVER_COMMINTERFACE_H_

#include "../PLATFORM.h"
#include "COMM.h"

class CommInterface
{
public:
	virtual ~CommInterface(){};
    virtual std::string onUserRead(uint8_t channel) = 0;
    virtual void onUserWrite(std::string data, uint8_t channel) = 0;
private:
};




#endif /* DRIVER_COMMINTERFACE_H_ */
