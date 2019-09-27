/*
 * Task.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_TASK_H_
#define UTILS_TASK_H_

class Task
{
public:
	virtual void run() = 0;	//is called endlessly by the TaskScheduler
private:

};



#endif /* UTILS_TASK_H_ */
