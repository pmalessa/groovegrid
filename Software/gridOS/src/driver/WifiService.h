#ifndef WIFISERVICE_H_
#define WIFISERVICE_H_


#include "../PLATFORM.h"
#include "DeltaTimer.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "lwip/err.h"
#include "lwip/sys.h"

//#define WIFI_SSID           "MG"
//#define WIFI_PASS           "QU;PkSsQVnksrUJ9"
#define WIFI_SSID "Otterbau"
#define WIFI_PASS "In1ThunfischWasserbueffel"

#define WIFI_MAXIMUM_RETRY  5

class WifiService{

 public:
	static WifiService& getInstance();
	virtual ~WifiService(void);
	bool isConnected();
	void run();
	void reconnect();

    static EventGroupHandle_t wifiEventGroup;
    static const int WIFI_CONNECTED_BIT = BIT0;
    static int retryCounter;
    static const char* tag;
 private:
	WifiService();
	WifiService(const WifiService&);
	WifiService& operator = (const WifiService&);

	DeltaTimer wifiTaskTimer;
	xTaskHandle wifiTask;
    bool connected = false;

    static void runWrapper(void* _this){((WifiService*)_this)->run();}
    static esp_err_t event_handler(void *ctx, system_event_t *event);
};

#endif /* WIFISERVICE_H_ */
