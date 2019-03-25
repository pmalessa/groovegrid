/*
 * EventManager.cpp
 *
 *  Created on: 23.03.2019
 *      Author: pmale
 */

#if not defined(AVR)
#include "EventManager.h"
#include <algorithm>

using namespace std;

void EventManager::Attach(Listener *listener)
{
    list.push_back(listener);
}
void EventManager::Detach(Listener *listener)
{
    list.erase(std::remove(list.begin(), list.end(), listener), list.end());
}

void EventManager::onEvent()
{
    for(vector<Listener*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->Update();
        }
    }
}

#endif
