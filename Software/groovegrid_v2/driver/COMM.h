/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef COMM_H_
#define COMM_H_

#include "../PLATFORM.h"
#include "../utils/Task.h"
#include "../utils/Vector.h"
#include "../utils/InputListener.h"

#define MAX_LISTENER_NUM 10

class COMM : public Task{
 public:
	enum InputType{
		MAIN,
		APP
	};

	static COMM& getInstance();
	virtual ~COMM(void);
	void         Attach(InputListener *obj, COMM::InputType inputType);
	void         Detach(InputListener *obj, COMM::InputType inputType);
	void run();

 private:
	COMM();
	COMM(const COMM&);
	COMM & operator = (const COMM &);

	void app_send(char byte);
	void main_send(char byte);

    Vector<InputListener*> mainlist;
    InputListener* mainstorage[MAX_LISTENER_NUM];
    Vector<InputListener*> applist;
    InputListener* appstorage[MAX_LISTENER_NUM];

};


#endif /* COMM_H_ */
