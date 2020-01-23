#ifndef WEBPAGES_H_
#define WEBPAGES_H_

#include <WebServer.h>
#include "eeprom.helper.h"

// extern Settings settings;

String htmlPage(String, String);
String indexHtml();
String configHtml(WebServer &, Settings &, bool);

#endif
