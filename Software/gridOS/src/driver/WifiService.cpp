
#include "WifiService.h"

const char* TAG = "WifiService";
DeltaTimer WifiService::wifiTaskTimer;
xTaskHandle WifiService::wifiTask;
EventGroupHandle_t WifiService::wifiEventGroup;
bool WifiService::isInitialized = false;
uint8_t WifiService::retryCounter = 0;

bool WifiService::isConnected()
{
    return xEventGroupGetBits(wifiEventGroup) & EVENT_BIT(wifiEvent::WIFI_CONNECTED);
}

void WifiService::init()
{
    if(!isInitialized){
        isInitialized = true;
        wifiTaskTimer.setTimeStep(TASK_TIMER_SECONDS*1000);
        wifiEventGroup = xEventGroupCreate();
        assert(wifiEventGroup != nullptr);

        tcpip_adapter_init();
        esp_event_loop_init(event_handler, NULL);

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
        ESP_LOGI(TAG, "initialized.");
    }
}

void WifiService::run()
{
    static DeltaTimer retryDelayTimer(-1); //disabled
	while(1)
	{
		if(wifiTaskTimer.isTimeUp())
		{
            if(WifiService::isConnected())
            {
                ESP_LOGI(TAG,"connected");
            }
            else
            {
                ESP_LOGI(TAG,"disconnected");
            }

            EventBits_t currentEvents = xEventGroupGetBits(wifiEventGroup);
            if(!(currentEvents & EVENT_BIT(wifiEvent::WIFI_CONNECTED))){    //if not connected
                if(retryCounter < MAX_RETRY){
                    retryCounter++;
                    ESP_LOGI(TAG,"Connect Retry %i/%i",retryCounter,MAX_RETRY);
                    ESP_ERROR_CHECK(esp_wifi_disconnect());
                    vTaskDelay(500);
                    ESP_ERROR_CHECK(esp_wifi_connect());
                    if(retryCounter == MAX_RETRY){
                        ESP_LOGW(TAG,"Couldnt connect after %i retries. Trying again after %i seconds",MAX_RETRY, RETRY_DELAY_SECONDS);
                        retryDelayTimer.setTimeStep(RETRY_DELAY_SECONDS*1000);
                    }
                }
            }
            
            if(xEventGroupGetBits(wifiEventGroup) & EVENT_BIT(wifiEvent::WIFI_RECONNECT)){ //Reconnect signal received
                ESP_LOGI(TAG,"Reconnect bit set");
                xEventGroupClearBits(wifiEventGroup,wifiEvent::WIFI_RECONNECT);
                retryCounter = 0;
                ESP_LOGI(TAG,"reconnecting...");
                ESP_ERROR_CHECK(esp_wifi_disconnect());
                vTaskDelay(500);
                ESP_ERROR_CHECK(esp_wifi_connect());
            }
            if(retryDelayTimer.isTimeUp()){
                retryDelayTimer.setTimeStep(-1); //disabled
                ESP_LOGI(TAG,"waited for %i seconds",RETRY_DELAY_SECONDS);
                reconnect();
            }
        }
        vTaskDelay(500);
    }
}

void WifiService::reconnect()
{
    ESP_LOGI(TAG,"WifiService::reconnect");
    xEventGroupSetBits(wifiEventGroup,wifiEvent::WIFI_RECONNECT);
}

void WifiService::enable(){
    ESP_LOGI(TAG,"WifiService::enable");
    xTaskCreatePinnedToCore((TaskFunction_t)run,"wifiTask", 2048, nullptr,1,&wifiTask,0);
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void WifiService::disable(){
    if( wifiTask != NULL )
    {
        vTaskDelete(wifiTask);
    }
    esp_wifi_stop();
}

bool WifiService::isEnabled(){
    return (xEventGroupGetBits(wifiEventGroup) & EVENT_BIT(wifiEvent::WIFI_ENABLED));
}

esp_err_t WifiService::event_handler(void *ctx, system_event_t *event)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFALSE;
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG,"SYSTEM_EVENT_STA_START");
        xResult = xEventGroupSetBitsFromISR(wifiEventGroup, wifiEvent::WIFI_ENABLED, &pxHigherPriorityTaskWoken); //Enabled=true
        xEventGroupClearBitsFromISR(wifiEventGroup, wifiEvent::WIFI_CONNECTED); //Connected=false
        break;
    case SYSTEM_EVENT_STA_STOP:
        ESP_LOGI(TAG,"SYSTEM_EVENT_STA_STOP");
        xEventGroupClearBitsFromISR(wifiEventGroup, wifiEvent::WIFI_ENABLED); //Enabled=false
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG,"SYSTEM_EVENT_STA_GOT_IP");
        xResult = xEventGroupSetBitsFromISR(wifiEventGroup, wifiEvent::WIFI_CONNECTED, &pxHigherPriorityTaskWoken); //Connected=true
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG,"SYSTEM_EVENT_STA_DISCONNECTED");
        {
            xEventGroupClearBitsFromISR(wifiEventGroup, wifiEvent::WIFI_CONNECTED);
            ESP_LOGI(TAG,"connect to the AP failed\n");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        }
    default:
        break;
    }
    if( xResult == pdPASS && pxHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
    return ESP_OK;
}