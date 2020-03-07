#include "GrooveWeb.h"

const char* GrooveWeb::tag = "Web";

esp_err_t webEventHandler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI("Web", "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI("Web", "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI("Web", "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI("Web", "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI("Web", "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI("Web", "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI("Web", "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}


GrooveWeb& GrooveWeb::getInstance()
{
	static GrooveWeb _instance;
	return _instance;
}
GrooveWeb::~GrooveWeb(){}

GrooveWeb::GrooveWeb()
{
    static WifiService& wifiService = WifiService::getInstance();
	UNUSED(wifiService);
    currentStatus = GrooveWeb::IDLE;
    xTaskHandle webTask = NULL;
    otaConfig = {
        .url = CONFIG_WEB_UPDATE_URL
        //.cert_pem = (char *)0,
    };
    beaconConfig.url = CONFIG_WEB_BEACON_URL;
    //beaconConfig.event_handler = webEventHandler;
    initConfig.url = CONFIG_WEB_INIT_URL;
    //initConfig.event_handler = webEventHandler;

	updateTimer.setTimeStep(CONFIG_OTA_INTERVAL_S*1000);
    beaconTimer.setTimeStep(CONFIG_BEACON_INTERVAL_S*1000);
    xTaskCreatePinnedToCore(runWrapper,"webTask", 1024*16, this,1,&webTask,0);
}

GrooveWeb::updateStatus GrooveWeb::getUpdateStatus()
{
    return currentStatus;
}

void GrooveWeb::startUpdateCheck()
{
    updateTimer.forceTimeUp();  //force update check
}

void GrooveWeb::webThread()
{
    esp_err_t status;

    while(1)
    {
        if(updateTimer.isTimeUp())
        {
            currentStatus = RUNNING;
            status = esp_https_ota(&otaConfig);

            switch (status)
            {
            case ESP_OK:
                currentStatus = DONE;
                break;
            default:
                currentStatus = ERROR;
                break;
            }
        }
        if(beaconTimer.isTimeUp())
        {
            std::string data = "{\"uuid\":\"123456\",\"status\" : 202,\"current_app\" : \"Ani\",\"current_ip\" : \"1233445\"}";
            esp_http_client_handle_t client = esp_http_client_init(&beaconConfig);
            esp_http_client_set_method(client,HTTP_METHOD_POST);
            esp_http_client_set_post_field(client,data.c_str(),data.length());

            esp_err_t err = esp_http_client_perform(client);

            if (err == ESP_OK) {
                ESP_LOGI(tag, "Beacon: Status = %d", esp_http_client_get_status_code(client));
            }
            esp_http_client_cleanup(client);
        }
        vTaskDelay(5000);
    }
}