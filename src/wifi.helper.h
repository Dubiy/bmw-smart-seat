#ifndef WIFI_HELPER_H_
#define WIFI_HELPER_H_

#include <WiFi.h>
#include "const.h"
#include "udp.helper.h"
#include <queue>


// extern Settings settings;
extern queue<struct task> q;

void wifiEventHandler(WiFiEvent_t, WiFiEventInfo_t);

#endif
