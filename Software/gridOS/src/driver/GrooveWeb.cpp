#include "GrooveWeb.h"

const char* GrooveWeb::tag = "Web";
const char* test_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFjTCCA3WgAwIBAgIRANOxciY0IzLc9AUoUSrsnGowDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTYxMDA2MTU0MzU1\n" \
"WhcNMjExMDA2MTU0MzU1WjBKMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
"RW5jcnlwdDEjMCEGA1UEAxMaTGV0J3MgRW5jcnlwdCBBdXRob3JpdHkgWDMwggEi\n" \
"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCc0wzwWuUuR7dyXTeDs2hjMOrX\n" \
"NSYZJeG9vjXxcJIvt7hLQQWrqZ41CFjssSrEaIcLo+N15Obzp2JxunmBYB/XkZqf\n" \
"89B4Z3HIaQ6Vkc/+5pnpYDxIzH7KTXcSJJ1HG1rrueweNwAcnKx7pwXqzkrrvUHl\n" \
"Npi5y/1tPJZo3yMqQpAMhnRnyH+lmrhSYRQTP2XpgofL2/oOVvaGifOFP5eGr7Dc\n" \
"Gu9rDZUWfcQroGWymQQ2dYBrrErzG5BJeC+ilk8qICUpBMZ0wNAxzY8xOJUWuqgz\n" \
"uEPxsR/DMH+ieTETPS02+OP88jNquTkxxa/EjQ0dZBYzqvqEKbbUC8DYfcOTAgMB\n" \
"AAGjggFnMIIBYzAOBgNVHQ8BAf8EBAMCAYYwEgYDVR0TAQH/BAgwBgEB/wIBADBU\n" \
"BgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEBATAwMC4GCCsGAQUFBwIB\n" \
"FiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQub3JnMB0GA1UdDgQWBBSo\n" \
"SmpjBH3duubRObemRWXv86jsoTAzBgNVHR8ELDAqMCigJqAkhiJodHRwOi8vY3Js\n" \
"LnJvb3QteDEubGV0c2VuY3J5cHQub3JnMHIGCCsGAQUFBwEBBGYwZDAwBggrBgEF\n" \
"BQcwAYYkaHR0cDovL29jc3Aucm9vdC14MS5sZXRzZW5jcnlwdC5vcmcvMDAGCCsG\n" \
"AQUFBzAChiRodHRwOi8vY2VydC5yb290LXgxLmxldHNlbmNyeXB0Lm9yZy8wHwYD\n" \
"VR0jBBgwFoAUebRZ5nu25eQBc4AIiMgaWPbpm24wDQYJKoZIhvcNAQELBQADggIB\n" \
"ABnPdSA0LTqmRf/Q1eaM2jLonG4bQdEnqOJQ8nCqxOeTRrToEKtwT++36gTSlBGx\n" \
"A/5dut82jJQ2jxN8RI8L9QFXrWi4xXnA2EqA10yjHiR6H9cj6MFiOnb5In1eWsRM\n" \
"UM2v3e9tNsCAgBukPHAg1lQh07rvFKm/Bz9BCjaxorALINUfZ9DD64j2igLIxle2\n" \
"DPxW8dI/F2loHMjXZjqG8RkqZUdoxtID5+90FgsGIfkMpqgRS05f4zPbCEHqCXl1\n" \
"eO5HyELTgcVlLXXQDgAWnRzut1hFJeczY1tjQQno6f6s+nMydLN26WuU4s3UYvOu\n" \
"OsUxRlJu7TSRHqDC3lSE5XggVkzdaPkuKGQbGpny+01/47hfXXNB7HntWNZ6N2Vw\n" \
"p7G6OfY+YQrZwIaQmhrIqJZuigsrbe3W+gdn5ykE9+Ky0VgVUsfxo52mwFYs1JKY\n" \
"2PGDuWx8M6DlS6qQkvHaRUo0FMd8TsSlbF0/v965qGFKhSDeQoMpYnwcmQilRh/0\n" \
"ayLThlHLN81gSkJjVrPI0Y8xCVPB4twb1PFUd2fPM3sA1tJ83sZ5v8vgFv2yofKR\n" \
"PB0t6JzUA81mSqM3kxl5e+IZwhYAyO0OTg3/fs8HqGTNKd9BqoUwSRBzp06JMg5b\n" \
"rUCGwbCUDI0mxadJ3Bz4WxR6fyNpBK2yAinWEsikxqEt\n" \
"-----END CERTIFICATE-----\n";



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


    otaConfig.cert_pem = (char *)test_root_ca;
    otaConfig.skip_cert_common_name_check = true;
 //   otaConfig.url = "https://groovegrid.de/api/device/update.bin";
    otaConfig.host = "groovegrid.de";
    otaConfig.port = 443;
    otaConfig.path = "api/device/update.bin";
    otaConfig.skip_cert_common_name_check = true;
    otaConfig.transport_type = HTTP_TRANSPORT_OVER_SSL;
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
            ESP_LOGI(tag, "OTA started");
            status = esp_https_ota(&otaConfig);

            switch (status)
            {
            case ESP_OK:
                ESP_LOGI(tag, "OTA successfull");
                currentStatus = DONE;
                break;
            default:
                ESP_LOGI(tag, "OTA failed");
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