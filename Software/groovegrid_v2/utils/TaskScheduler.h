/*
 * TaskScheduler.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_TASKSCHEDULER_H_
#define UTILS_TASKSCHEDULER_H_
#include "Vector.h"
#include "Task.h"

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
    Vector<Task*> list;
    Task* storage[MAX_TASK_NUM];
};



#endif /* UTILS_TASKSCHEDULER_H_ */
