
#include "WifiService.h"

int WifiService::retryCounter = 0;
EventGroupHandle_t WifiService::wifiEventGroup;
const char* WifiService::tag;


WifiService& WifiService::getInstance()
{
    static WifiService _instance;
	return _instance;
}

bool WifiService::isConnected()
{
    return connected;
}

WifiService::~WifiService(){}
WifiService::WifiService()
{
    tag = "wifiService";
	wifiTaskTimer.setTimeStep(3000);

	xTaskCreatePinnedToCore(runWrapper,"wifiTask", 2048, this,1,&wifiTask,0);

    wifiEventGroup = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(tag, "WifiService initialized.");
}

void WifiService::run()
{
	while(1)
	{
		if(wifiTaskTimer.isTimeUp())
		{
            if(connected == true)
            {
                ESP_LOGI(tag,"Wifi connected");
            }
            else
            {
                ESP_LOGI(tag,"Wifi disconnected");
            }
        }
        vTaskDelay(100);
    }
}

void WifiService::reconnect()
{
    esp_wifi_disconnect();
    esp_wifi_connect();
}

esp_err_t WifiService::event_handler(void *ctx, system_event_t *event)
{
    WifiService& wifiService = WifiService::getInstance();
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        wifiService.connected = false;
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        wifiService.connected = true;
//        ESP_LOGI(tag, "got ip:%s",
//                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        retryCounter = 0;
        xEventGroupSetBits(wifiEventGroup, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        {
            wifiService.connected = false;
            if (retryCounter < WIFI_MAXIMUM_RETRY) {
                esp_wifi_connect();
                xEventGroupClearBits(wifiEventGroup, WIFI_CONNECTED_BIT);
                retryCounter++;
                ESP_LOGI(tag,"retry to connect to the AP");
            }
            ESP_LOGI(tag,"connect to the AP failed\n");
            break;
        }
    default:
        break;
    }
    return ESP_OK;
}