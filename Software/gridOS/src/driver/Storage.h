
#ifndef DRIVER_STORAGE_H_
#define DRIVER_STORAGE_H_

#include "PLATFORM.h"
#include "nvs_flash.h"
#include "nvs.h"

class Storage{
public:
	static void init();

    #define SET_BRIGHTNESS "Brightness"
    #define SET_USERCOLOR "UserColor"
    #define SET_INITVALUE "InitValue"

    static uint32_t getValue(const char *key);
    static void setValue(const char *key, uint32_t value);
    static void eraseStorage();
private:
    static nvs_handle_t storageHandler;
};

#endif