/*
 * TaskScheduler.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_TASKSCHEDULER_H_
#define UTILS_TASKSCHEDULER_H_
#include "Task.h"
#include <vector>

#define MAX_TASK_NUM 10

//Singleton
class TaskScheduler
{
public:
	static TaskScheduler& getInstance();
	~TaskScheduler();
    void Attach(Task *task);
    void Detach(Task *task);
    void handleTasks();
private:
    TaskScheduler();
    TaskScheduler(const TaskScheduler&);
    TaskScheduler & operator = (const TaskScheduler &);

    std::vector<Task*> list;
};



#endif /* UTILS_TASKSCHEDULER_H_ */
