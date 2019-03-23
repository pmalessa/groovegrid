/*
 * EventManager.h
 *
 *  Created on: 23.03.2019
 *      Author: pmale
 */

#ifndef UTILS_EVENTMANAGER_H_
#define UTILS_EVENTMANAGER_H_

#include "Listener.h"
#include <stdio.h>
#include <vector>
#include <algorithm>

class EventManager {
public:
	void addListener( Listener* );
	void removeListener( Listener* );

protected:
	std::vector<Listener*> mListeners;
	virtual void dispatchEvent();
};

#endif /* UTILS_EVENTMANAGER_H_ */
