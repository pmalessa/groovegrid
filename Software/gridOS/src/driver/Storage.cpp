/*
 * Microphone.cpp
 *
 *  Created on: 16.08.2019
 *      Author: pmale
 */

#include "Storage.h"

static const char* TAG = "Storage";
nvs_handle_t Storage::storageHandler;

void Storage::init()
{
    // Initialize NVS
	ESP_LOGI(TAG,"Storage Init");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
		ESP_LOGI(TAG,"NVS Corrupted, erased");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
		return;
    }
    ESP_ERROR_CHECK( err );
	ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &storageHandler));

	uint32_t tmp;
	if(nvs_get_u32(storageHandler, "InitValue", &tmp) != ESP_OK && tmp != 0xDEADBEEF)
	{
		//set default values
		ESP_LOGI(TAG,"Setting default values");
		setValue(SET_USERCOLOR, 0xFF0000FF);
		setValue(SET_BRIGHTNESS,0xFE);
		setValue(SET_INITVALUE,0xDEADBEEF);
	}
}

void Storage::setValue(const char *key, uint32_t value)
{
    ESP_ERROR_CHECK(nvs_set_u32(storageHandler, key, value));
	ESP_ERROR_CHECK(nvs_commit(storageHandler));
}

uint32_t Storage::getValue(const char *key)
{
	uint32_t tmp;
	ESP_ERROR_CHECK(nvs_get_u32(storageHandler, key, &tmp));
	return tmp;
}