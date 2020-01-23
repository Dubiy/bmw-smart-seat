#ifndef WEBSERVER_HELPER_H_
#define WEBSERVER_HELPER_H_

#include <WebServer.h>
#include "eeprom.helper.h"
#include "webpages.h"


extern WebServer webServer;
extern Settings settings;

void initWebserver();

#endif