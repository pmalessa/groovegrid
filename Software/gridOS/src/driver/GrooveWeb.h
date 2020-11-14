
#ifndef DRIVER_GROOVEWEB_H_
#define DRIVER_GROOVEWEB_H_

#include "PLATFORM.h"
#include "DeltaTimer.h"
#include "WifiService.h"
#include "mdns.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "tcpip_adapter.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include "MessageService.h"

class GrooveWeb{
public:
	static GrooveWeb& getInstance();
	~GrooveWeb(void);
    void webThread();
private:
	GrooveWeb();
	GrooveWeb(const GrooveWeb&);
	GrooveWeb & operator = (const GrooveWeb &);
	static void runWrapper(void* _this){((GrooveWeb*)_this)->webThread();}
};

#endif