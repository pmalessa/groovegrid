/*
 * Listener.h
 *
 *  Created on: 23.03.2019
 *      Author: pmale
 */

#ifndef UTILS_LISTENER_H_
#define UTILS_LISTENER_H_

#if not defined(AVR)

class Listener
{
public:
	virtual void Update() = 0;
};


#endif
#endif /* UTILS_LISTENER_H_ */
