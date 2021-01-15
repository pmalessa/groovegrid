
#ifndef DRIVER_STORAGE_H_
#define DRIVER_STORAGE_H_

#include "PLATFORM.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <sys/fcntl.h>
#include "esp_spiffs.h"
#include <string.h>

class Storage{
public:
    #define SET_BRIGHTNESS "Brightness"
    #define SET_USERCOLOR "UserColor"
    #define SET_INITVALUE "InitValue"

    #define FNAME_CONFIG "/spiffs/config.json"
    
	static void init();
    static void unmount();
    static uint32_t getConfig(const char *key);
    static void setConfig(const char *key, uint32_t value);
    static void eraseConfig();
    static void printConfigInfo();
private:
    static nvs_handle_t storageHandler;
};

#endif