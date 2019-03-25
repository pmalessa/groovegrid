/*
 * EventManager.h
 *
 *  Created on: 23.03.2019
 *      Author: pmale
 */

#ifndef UTILS_EVENTMANAGER_H_
#define UTILS_EVENTMANAGER_H_

#if not defined(AVR)
#include <vector>
#include <list>
#include "Listener.h"

class EventManager
{
    std::vector<Listener*> list;

public:
    void Attach(Listener *listener);
    void Detach(Listener *listener);
    void onEvent();
};
#endif

#endif /* UTILS_EVENTMANAGER_H_ */
