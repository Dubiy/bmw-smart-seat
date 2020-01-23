#ifndef EEPROM_HELPER_H_
#define EEPROM_HELPER_H_

#include <EEPROM.h>
#include <WiFi.h>
#include "types.h"

extern uint32_t executeUntil;

byte CRC(const String);
void setEEPROMString(int, int, String);
String getEEPROMString(int, int);
Settings getSettings();
void setSettings(Settings &);
String urldecode(const char *);


#endif