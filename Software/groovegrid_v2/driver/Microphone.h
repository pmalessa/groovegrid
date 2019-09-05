/*
 * Microphone.h
 *
 *  Created on: 16.08.2019
 *      Author: pmale
 */

#ifndef DRIVER_MICROPHONE_H_
#define DRIVER_MICROPHONE_H_

#include "../PLATFORM.h"
#include <driver/i2s.h>
#include "arduinoFFT.h"

class Microphone {

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
#define SAMPLES 64

 public:
	static Microphone& getInstance();
	~Microphone(void);
	bool isFFTAvailable();
	void computeFFT();
	void printFFT();
	int32_t getCurrentSoundLevel(void);
	uint32_t read(uint32_t *buf, uint32_t length);

 private:
Microphone();
Microphone(const Microphone&);
Microphone & operator = (const Microphone &);

bool initialized = false, fftAvailable = false;
const double samplingFrequency = 100; //Hz, must be less than 10000 due to ADC
double vReal[SAMPLES];
double vImag[SAMPLES];
arduinoFFT *fft;
};



#endif /* DRIVER_MICROPHONE_H_ */
