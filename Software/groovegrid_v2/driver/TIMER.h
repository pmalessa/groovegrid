/*
 * TIMER.h
 *
 *  Created on: 20.03.2019
 *      Author: pmale
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../PLATFORM.h"

void TIMER_Init();
void TIMER_attach(void (*isr)(), unsigned long microseconds);



#endif /* TIMER_H_ */
