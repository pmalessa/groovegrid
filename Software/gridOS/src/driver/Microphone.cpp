/*
 * Microphone.cpp
 *
 *  Created on: 16.08.2019
 *      Author: pmale
 */

#include "Microphone.h"


Microphone& Microphone::getInstance()
{
	static Microphone _instance;
	return _instance;
}


Microphone::~Microphone(){}
Microphone::Microphone()
{
	fftCounter = 0;

	esp_err_t err;

	// The I2S config as per the example
	const i2s_config_t i2s_config = {
	  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
	  .sample_rate = SAMPLERATE_HZ,                         // 44.1KHz
	  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // could only get it to work with 32bits
	  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // although the SEL config should be left, it seems to transmit on right
	  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
	  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt level 1
	  .dma_buf_count = 4,                           // number of buffers
	  .dma_buf_len = 80                              // 80 samples per buffer (minimum)
	};

	// The pin config as per the setup
	const i2s_pin_config_t pin_config = {
	  .bck_io_num = 26,   // BCKL
	  .ws_io_num = 25,    // LRCL
	  .data_out_num = -1, // not used (only for speakers)
	  .data_in_num = 21   // DOUT
	};

	err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	if (err != ESP_OK) {
		ESP_LOGE("Mic","Failed installing driver: %d\n", err);
		return;
	}
	err = i2s_set_pin(I2S_NUM_0, &pin_config);
	if (err != ESP_OK) {
		ESP_LOGE("Mic","Failed setting pin: %d\n", err);
		return;
	}
	ESP_LOGI("Mic","I2S driver installed.");
	initialized = true;
	xTaskCreatePinnedToCore(runWrapper,"micTask", 2048, this,1,&micTask,0);
	micTaskTimer.setTimeStep(3000);	//every second

	fft = new arduinoFFT(fftBuffer.real, fftBuffer.imag, NR_FFT_SAMPLES, SAMPLERATE_HZ);
	fft->Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
}

void Microphone::computeFFT()
{
	uint32_t bytes_read; uint64_t mean = 0;
	i2s_read(I2S_NUM_0, micSample, NR_FFT_SAMPLES*BYTES_PER_SAMPLE, &bytes_read, portMAX_DELAY);	//no timeout, wait till all samples are read
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)	//calc mean value
	{
		mean += micSample[i];
	}
	mean = mean / NR_FFT_SAMPLES;
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{
		micSample[i] -= mean;				//remove mean
		if(micSample[i] > maxAmplitude)
		{
			maxAmplitude = micSample[i];	//get max Amplitude
		}
	}
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)	//uint32_t to double, maybe scaling required
	{
		fftBuffer.real[i] = ((double) micSample[i]) / maxAmplitude;
		fftBuffer.imag[i] = 0;
	}
	fft->Compute(FFT_FORWARD); /* Compute FFT */
	fft->ComplexToMagnitude(); /* Compute magnitudes, saved in real value */
	//LOCK NECESSARY
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{
		fftResult.sample[i] = fftBuffer.real[i];	//save result
	}
	fftResult.majorPeak = fft->MajorPeak();
	//UNLOCK
	fftCounter++; //count for ffts per second measure	
}

void Microphone::run()
{
	while(1)
	{
		if(micTaskTimer.isTimeUp()) //report every second
		{
			ESP_LOGI("Mic", "majorPeak: %f Hz",fftResult.majorPeak);
			ESP_LOGI("Mic","%i ffts per sec",fftCounter/3);
			fftCounter = 0;
		}
		vTaskDelay(1);
	}
}

bool Microphone::isFFTAvailable()
{
	return fftAvailable;
}

void Microphone::getFFT(double *fftBuffer)
{
	computeFFT();
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{
		*(fftBuffer+i) = fftResult.sample[i];
	}
}
