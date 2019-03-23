/*
 * EventManager.cpp
 *
 *  Created on: 23.03.2019
 *      Author: pmale
 */


#include "EventManager.h"

void EventManager::addListener( Listener *l )
{
	mListeners.push_back(l);
}

void EventManager::removeListener( Listener *l )
{
	mListeners.erase( std::remove( mListeners.begin(), mListeners.end(), l ), mListeners.end() );
	//having toruble with the rmove function...
}

void EventManager::dispatchEvent()
{
	for( std::vector<Listener*>::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener  )
	{
		(*listener)->handleEvent();
	}
}

