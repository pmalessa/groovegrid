/*
 * TaskScheduler.cpp
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#include "TaskScheduler.h"

TaskScheduler::TaskScheduler()
{
}

TaskScheduler::~TaskScheduler()
{
}

TaskScheduler& TaskScheduler::getInstance()
{
	static TaskScheduler _instance;
	return _instance;
}

void TaskScheduler::Attach(Task *task)
{
    list.push_back(task);
}
void TaskScheduler::Detach(Task *task)
{
	for (uint16_t i=0; i < list.size(); i++) {
		if(list.at(i) == task)
		{
			list.erase(list.begin()+i);
		}
	}
}

//Call run function of each Task before returning
void TaskScheduler::handleTasks()
{
	if(!list.empty())
	{
		for(uint16_t i=0; i<list.size();i++)
		{
			list.at(i)->run();
		}
	}
}
