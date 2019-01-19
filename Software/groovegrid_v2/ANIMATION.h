/*
 * ANIMATION.h
 *
 *  Created on: 22.09.2017
 *      Author: pmale
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

void ANIMATION_randomPixels(Adafruit_NeoMatrix *matrix);
void ANIMATION_randomLines(Adafruit_NeoMatrix *matrix);
void ANIMATION_randomPulseRects();
void ANIMATION_vRunner();
void ANIMATION_vBoot();


#endif /* ANIMATION_H_ */
