/*
 * COMM.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef COMM_H_
#define COMM_H_

#include "../PLATFORM.h"


class COMM {
 public:
	enum Event{
		SWIPEDIRECTION,
		GAMESTATE,
		SERIALINPUT
	};

	static COMM& getInstance();
	~COMM(void);
	int 		 read();
	void         setCallback(void *functionPointer, COMM::Event eventType);
	void         removeCallback(void *functionPointer);

 private:
	COMM();
	COMM(const COMM&);
	COMM & operator = (const COMM &);

};


#endif /* COMM_H_ */
