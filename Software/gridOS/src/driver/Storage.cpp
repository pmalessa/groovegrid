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

	printConfigInfo();

	uint32_t tmp;
	if(nvs_get_u32(storageHandler, "InitValue", &tmp) == ESP_OK)
	{
		if(tmp != 0xDEADBEEF)
		{
			//set default values
			ESP_LOGI(TAG,"Setting default values");
			setConfig(SET_USERCOLOR, 0xAA5500FF);
			setConfig(SET_BRIGHTNESS,0x30);
			setConfig(SET_INITVALUE,0xDEADBEEF);
		}
	}
	else
	{
		//set default values
		ESP_LOGI(TAG,"Setting default values");
		setConfig(SET_USERCOLOR, 0xAA5500FF);
		setConfig(SET_BRIGHTNESS,0x30);
		setConfig(SET_INITVALUE,0xDEADBEEF);
	}
	
	//spiffs
ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = "storage",
      .max_files = 5,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file %s",FNAME_CONFIG);
    FILE* f = fopen(FNAME_CONFIG, "r");
    if (f != NULL) {
		char line[300];
		fgets(line, sizeof(line), f);
		fclose(f);
		// strip newline
		char* pos = strchr(line, '\n');
		if (pos) {
			*pos = '\0';
		}
		ESP_LOGI(TAG, "Read from file: '%s'", line);
    }
	else
	{
		ESP_LOGE(TAG, "Failed to open file for reading");
	}
}

void Storage::unmount(){
	esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = "storage",
      .max_files = 5,
      .format_if_mount_failed = false
    };
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

void Storage::setConfig(const char *key, uint32_t value)
{
    ESP_ERROR_CHECK(nvs_set_u32(storageHandler, key, value));
	ESP_ERROR_CHECK(nvs_commit(storageHandler));
}

uint32_t Storage::getConfig(const char *key)
{
	uint32_t tmp;
	ESP_ERROR_CHECK(nvs_get_u32(storageHandler, key, &tmp));
	return tmp;
}

void Storage::eraseConfig()
{
	ESP_ERROR_CHECK(nvs_flash_erase());
	ESP_LOGI(TAG,"Storage erased");
}

void Storage::printConfigInfo()
{
	ESP_LOGI(TAG,"Storage Info:");
	nvs_iterator_t it = nvs_entry_find("nvs", "storage", NVS_TYPE_ANY);
	while (it != NULL) {
		nvs_entry_info_t info;
 		nvs_entry_info(it, &info);
 		it = nvs_entry_next(it);
		uint32_t value = getConfig(info.key);
 		ESP_LOGI(TAG,"key '%s', value 0x%X", info.key, value);
 	};
}