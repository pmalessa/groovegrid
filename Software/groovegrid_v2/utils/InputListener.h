/*
 * InputListener.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef UTILS_INPUTLISTENER_H_
#define UTILS_INPUTLISTENER_H_

#include "../PLATFORM.h"

class InputListener
{
public:
	virtual void onInput(char *data) = 0;
};



#endif /* UTILS_INPUTLISTENER_H_ */
