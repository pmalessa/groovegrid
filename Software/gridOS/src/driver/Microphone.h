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
#include <arduinoFFT.h>
#include "DeltaTimer.h"

class Microphone {

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

#define NR_FFT_SAMPLES 256
#define SAMPLERATE_HZ 16000
#define BYTES_PER_SAMPLE 4

 public:
	static Microphone& getInstance();
	~Microphone(void);
	bool isFFTAvailable();
	void computeFFT();
	void printFFT();
	int32_t getCurrentSoundLevel(void);
	uint32_t read(uint32_t *buf, uint32_t length);
	void run();
	void getFFT(double *fftBuffer);

 private:
	Microphone();
	Microphone(const Microphone&);
	Microphone & operator = (const Microphone &);
	static void runWrapper(void* _this){((Microphone*)_this)->run();}
	void processMicSample();

	bool initialized = false, fftAvailable = false;
	arduinoFFT *fft;
	xTaskHandle micTask;
	DeltaTimer micTaskTimer;

	struct {
		double real[NR_FFT_SAMPLES];
		double imag[NR_FFT_SAMPLES];
	}fftBuffer;
	uint32_t micSample[NR_FFT_SAMPLES];
	struct {
		double sample[NR_FFT_SAMPLES];
		double majorPeak;
	}fftResult;
	uint32_t fftCounter;
	uint32_t maxAmplitude = 0;
	uint32_t maxFFTValue = 0;
};



#endif /* DRIVER_MICROPHONE_H_ */
