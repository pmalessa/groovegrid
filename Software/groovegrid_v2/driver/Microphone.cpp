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
	esp_err_t err;

	// The I2S config as per the example
	const i2s_config_t i2s_config = {
	  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
	  .sample_rate = 16000,                         // 16KHz
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

	// Configuring the I2S driver and pins.
	// This function must be called before any I2S driver read/write operations.
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
	ESP_LOGI("I2S driver installed.");
	initialized = true;

	//fft = new arduinoFFT(vReal, vImag, SAMPLES, samplingFrequency);
	//fft->Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
}

bool Microphone::isFFTAvailable()
{
	return fftAvailable;
}

void Microphone::computeFFT()
{
	uint32_t sample, bytes_read;
	if(initialized)
	{
		for(uint16_t i=0;i<SAMPLES;i++)
		{
			i2s_read(I2S_NUM_0, &sample, 4, &bytes_read, 0);
			if(bytes_read > 0)
			{
				vReal[i] = (sample >> 14); //32 -> 18bit, as the sensor is only 18bit
			}
			else
			{
				return;
			}
		}
		//fft->Compute(FFT_FORWARD); /* Compute FFT */
		//fft->ComplexToMagnitude(); /* Compute magnitudes */
		fftAvailable = true;
	}
}

void Microphone::printFFT()
{
	  for (uint16_t i = 0; i < (SAMPLES>>2); i++)
	  {
	    double abscissa = ((i * 1.0 * samplingFrequency) / SAMPLES);

	    //Serial.print(abscissa, 6);
	    //Serial.print("Hz");
	    //Serial.print(" ");
	    //Serial.println(vReal[i], 4);
	  }
	  //Serial.println();
}

int32_t Microphone::getCurrentSoundLevel()
{
	static uint32_t bytes_read;
	uint32_t sample;
	if(initialized)
	{
		i2s_read(I2S_NUM_0, &sample, 4, &bytes_read, 0);
		return sample;
	}
	else
	{
		return 0;
	}
}

uint32_t Microphone::read(uint32_t *buf, uint32_t length)
{
	uint32_t bytes_read;
	i2s_read(I2S_NUM_0, buf, length*4, &bytes_read, 0);
	return bytes_read/4;
}

