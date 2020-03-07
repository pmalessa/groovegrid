
#ifndef DRIVER_GROOVEWEB_H_
#define DRIVER_GROOVEWEB_H_

#include "PLATFORM.h"
#include "esp_https_ota.h"
#include "DeltaTimer.h"
#include "WifiService.h"

class GrooveWeb{
public:
	static GrooveWeb& getInstance();
	~GrooveWeb(void);
    typedef enum{
        IDLE,
        RUNNING,
        DONE,
        ERROR
    }updateStatus;
    void startUpdateCheck();
    updateStatus getUpdateStatus();
    void webThread();
private:
	GrooveWeb();
	GrooveWeb(const GrooveWeb&);
	GrooveWeb & operator = (const GrooveWeb &);
	static void runWrapper(void* _this){((GrooveWeb*)_this)->webThread();}
    

    updateStatus currentStatus;
    static const char* tag;
	xTaskHandle webTask;
    esp_http_client_config_t otaConfig, beaconConfig, initConfig;
    DeltaTimer updateTimer, beaconTimer;
};

#endif