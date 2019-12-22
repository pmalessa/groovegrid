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

#define NR_FFT_SAMPLES 64
#define SAMPLERATE_HZ 16000
#define BYTES_PER_SAMPLE 4

#define NR_BITS_PER_SAMPLE 18
#define BIT_SHIFT (32-NR_BITS_PER_SAMPLE)

#define SAMPLE_MEAN_SIZE 16384	//around 1 second, power of 2 for performance

#define ATTACK_TIME 0.04
#define RELEASE_TIME 0.0005 //0.00001
#define K_GAIN 0.04

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
	void processMicSamples();
	void readSamples();
	void initValues();

	bool initialized = false, fftAvailable = false;
	arduinoFFT *fft;
	xTaskHandle micTask;
	DeltaTimer micTaskTimer;

	struct {
		double real[NR_FFT_SAMPLES];
		double imag[NR_FFT_SAMPLES];
	}fftBuffer;
	int32_t micSample[NR_FFT_SAMPLES];
	double processedSample[NR_FFT_SAMPLES];
	int32_t mean;
	struct {
		double sample[NR_FFT_SAMPLES];
		double majorPeak;
	}fftResult;
	uint32_t fftCounter;
	double peakValue = 0;
	double gainValue = 0;
};



#endif /* DRIVER_MICROPHONE_H_ */
