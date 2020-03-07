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
	  .sample_rate = SAMPLERATE_HZ,
	  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // could only get it to work with 32bits
	  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // although the SEL config should be left, it seems to transmit on right
	  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
	  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt level 1
	  .dma_buf_count = 4,                           // number of buffers
	  .dma_buf_len = 80,                              // 80 samples per buffer (minimum)
	  .use_apll = 0,
	  .tx_desc_auto_clear = 0,
	  .fixed_mclk = 0
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
	initValues();
}

void Microphone::initValues()
{	//initial Values, take the first 64 samples
	readSamples();
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//calculate mean, incremental average
		mean = ((SAMPLE_MEAN_SIZE-1)*mean + micSample[i])/SAMPLE_MEAN_SIZE;	
	}
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//remove mean / DC Offset
		micSample[i] -= mean;				
	}
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//estimate peak value, EWMA
		if(abs(micSample[i]) > peakValue)
		{	//Attack
			peakValue = (1 - ATTACK_TIME)*peakValue + ATTACK_TIME*abs(micSample[i]);
		}
		else
		{	//Release
			peakValue = (1 - RELEASE_TIME)*peakValue;
		}
	}
	gainValue = peakValue;
}

void Microphone::readSamples()
{
	uint32_t bytes_read;
	i2s_read(I2S_NUM_0, micSample, NR_FFT_SAMPLES*BYTES_PER_SAMPLE, &bytes_read, portMAX_DELAY);	//no timeout, wait till all samples are read
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{
		micSample[i] = micSample[i] >> BIT_SHIFT; //only 18bits are data, rest zeros
	}
}

void Microphone::processMicSamples()
{
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//calculate mean, incremental average
		mean = ((SAMPLE_MEAN_SIZE-1)*mean + micSample[i])/SAMPLE_MEAN_SIZE;	
	}
	//ESP_LOGI("Mic","%i - %x",micSample[10],micSample[10]);
	//ESP_LOGI("Mic","mean: %i",mean);

	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//remove mean / DC Offset
		micSample[i] -= mean;				
	}

	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//estimate peak value, EWMA
		if(abs(micSample[i]) > peakValue)
		{	//Attack
			peakValue = (1 - ATTACK_TIME)*peakValue + ATTACK_TIME*abs(micSample[i]);
		}
		else
		{	//Release
			peakValue = (1 - RELEASE_TIME)*peakValue;
		}
	}

	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)
	{	//calculate gain smoothing factor, EWMA
		gainValue = (1 - K_GAIN)*gainValue + K_GAIN*peakValue;
		processedSample[i] = (double) micSample[i] / gainValue;	//correct gain factor, save processed sample
	}
	//ESP_LOGI("Mic","processed: %f",processedSample[10]);
}

void Microphone::computeFFT()
{
	readSamples();
	processMicSamples();
	for(uint16_t i=0;i<NR_FFT_SAMPLES;i++)	//uint32_t to double, maybe scaling required
	{
		fftBuffer.real[i] = processedSample[i];
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
