#ifndef PROVISIONSERVICE_H_
#define PROVISIONSERVICE_H_

#include "PLATFORM.h"
#include "GrooveTransition.h"
#include "driver/WifiService.h"

#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>

class ProvisionApp: public GrooveTransition{
public:
	ProvisionApp(GridTile* gridTile);
	~ProvisionApp();
	void run();
	bool isProvisioned();
private:
	DeltaTimer fadeTimer;
};


#endif /* PROVISIONSERVICE_H_ */
