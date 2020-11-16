#include "ProvisionApp.h"

static const char *TAG = "ProvisionApp";

/* Signal Wi-Fi events on this event-group */
const int WIFI_CONNECTED_EVENT = BIT0;
static EventGroupHandle_t wifi_event_group;

/* Event handler for catching system events */
static void event_handler(void* arg, esp_event_base_t event_base,
                          int event_id, void* event_data)
{
    static WifiService& wifiService = WifiService::getInstance();
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
            case WIFI_PROV_START:
                ESP_LOGI(TAG, "Provisioning started");
                break;
            case WIFI_PROV_CRED_RECV: {
                wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
                ESP_LOGI(TAG, "Received Wi-Fi credentials"
                         "\n\tSSID     : %s\n\tPassword : %s",
                         (const char *) wifi_sta_cfg->ssid,
                         (const char *) wifi_sta_cfg->password);
                break;
            }
            case WIFI_PROV_CRED_FAIL: {
                wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
                ESP_LOGE(TAG, "Provisioning failed!\n\tReason : %s"
                         "\n\tPlease reset to factory and retry provisioning",
                         (*reason == WIFI_PROV_STA_AUTH_ERROR) ?
                         "Wi-Fi station authentication failed" : "Wi-Fi access-point not found");
                wifi_prov_mgr_deinit();
                Storage::eraseStorage();
                esp_restart();
                break;
            }
            case WIFI_PROV_CRED_SUCCESS:
                ESP_LOGI(TAG, "Provisioning successful");
                break;
            case WIFI_PROV_END:
                /* De-initialize manager once provisioning is finished */
                wifi_prov_mgr_deinit();
                if(!wifiService.isConnected())
                {
                    wifiService.reconnect();
                }
                break;
            default:
                break;
        }
    }
}

ProvisionApp::ProvisionApp(GridTile *tile): GrooveTransition(tile) {

    static WifiService& wifiService = WifiService::getInstance();
	UNUSED(wifiService);

	running = true;
	fadeTimer.setTimeStep(5);

    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    wifi_prov_mgr_config_t config = {
    .scheme = wifi_prov_scheme_ble,
    .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM
    };
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

    if(isProvisioned())
    {
        running = false;    //skip Provisioning
        ESP_LOGI(TAG, "Already provisioned");
        wifi_prov_mgr_deinit();
        wifiService.reconnect();
        return;
    }
    ESP_LOGI(TAG, "Starting provisioning");

    char *service_name;
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);
    if (-1 == asprintf(&service_name, "%s-%02X%02X", "Groovegrid", mac[4], mac[5])) {
        abort();
    }

    wifi_prov_security_t security = WIFI_PROV_SECURITY_0;   //later sec1

    const char *pop = NULL; //later proof of possesion

    const char *service_key = NULL; //not used in BLE mode

    uint8_t custom_service_uuid[] = {
        /* LSB <---------------------------------------
        * ---------------------------------------> MSB */
        0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf,
        0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02,
    };
    wifi_prov_scheme_ble_set_service_uuid(custom_service_uuid);

    /* Start provisioning service */
    ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, service_key));

}

ProvisionApp::~ProvisionApp(){

}

void ProvisionApp::run()
{
    static CRGBW currentColor = CRGBW(0,0,0,0);
    static CRGBW desiredColor = CRGBW(0xFF,0,0xFF,0xFF);
	static uint8_t state = 0;
    static uint16_t currentDelay = 0;
    static uint16_t delayFrames = 500/FRAMERATE_MS;	//1second delay

    switch (state)
    {
    case 0:	//Logo fade in
        if(fadeTimer.isTimeUp())
        {
            if(currentColor.incrementUntil(desiredColor) == true)
            {
                state = 1;
            }
        }
        break;
    case 1: //delay
        if(currentDelay++ > delayFrames)
        {
            currentDelay = 0;
            state = 2;
        }
        break;
    case 2: //Logo Fade out
        if(fadeTimer.isTimeUp())
        {
            if(currentColor.decrementUntil(CRGBW(0,0,0,0)) == true)
            {
                state = 3;
            }
        }
        break;
    case 3: //delay
        if(isProvisioned())
        {
            running = false;
        }
        if(currentDelay++ > delayFrames)
        {
            currentDelay = 0;
            state = 0;
        }
    }
	if(running && frameTimer.isTimeUp())
	{
        tile->writeString(tile->getWidth()>>1,tile->getHeight()>>1,"Gg",currentColor,CRGBW(0,0,0,0),1);
		tile->endWrite();
	}
}

bool ProvisionApp::isProvisioned()
{
    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));
    return provisioned;
}