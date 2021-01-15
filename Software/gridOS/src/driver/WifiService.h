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

class WifiService{

 public:
	static void init();
	static bool isConnected();
	static void reconnect();
	static void enable();
	static void disable();
	static bool isEnabled();

	#define MAX_RETRY  5
	#define RETRY_DELAY_SECONDS 30
	#define TASK_TIMER_SECONDS 10
 private:
	typedef enum{
		WIFI_ENABLED = 1,
		WIFI_CONNECTED,
		WIFI_RECONNECT
	}wifiEvent;

	static void run();
	static esp_err_t event_handler(void *ctx, system_event_t *event);

	static DeltaTimer wifiTaskTimer;
	static xTaskHandle wifiTask;
	static EventGroupHandle_t wifiEventGroup;
	static bool isInitialized;
	static uint8_t retryCounter;
};

#endif /* WIFISERVICE_H_ */
