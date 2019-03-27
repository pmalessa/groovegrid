/*
 * TaskScheduler.cpp
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#include "TaskScheduler.h"
#include <algorithm>

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
    list.erase(std::remove(list.begin(), list.end(), task), list.end());
}

//Call run function of each Task before returning
void TaskScheduler::handleTasks()
{
    for(std::vector<Task*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0)
        {
            (*iter)->run();
        }
    }
}
