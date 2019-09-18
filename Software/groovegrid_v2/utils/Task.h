/*
 * Task.h
 *
 *  Created on: 27.03.2019
 *      Author: pmale
 */

#ifndef UTILS_TASK_H_
#define UTILS_TASK_H_

#include "../PLATFORM.h"

class Task
{
public:
	#define DEFAULT_STACKDEPTH 2048
	#define DEFAULT_PRIORITY 1
	virtual ~Task()
	{
		stop();
	}
	virtual void run() = 0;	//is called endlessly by the TaskScheduler
	static void runWrapper(void* _this){((Task*)_this)->run();}
	virtual void start()
	{
		if(taskHandle == NULL)
		{ //thunfisch
			if(tag == NULL)tag="Unnamed";
			Serial.print("Task");
			Serial.print(tag);
			Serial.println("start");
			xTaskCreate(runWrapper, (const char*)tag, stackDepth, this, priority, &taskHandle);
		}
	};
	virtual void stop()
	{
		if(taskHandle != NULL)
		{
			vTaskDelete(taskHandle);
		}
	};
protected:
	char* tag = NULL;
	unsigned int stackDepth = DEFAULT_STACKDEPTH;
	unsigned int priority = DEFAULT_PRIORITY;
private:
	xTaskHandle taskHandle = NULL;

};



#endif /* UTILS_TASK_H_ */
