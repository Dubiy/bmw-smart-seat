#ifndef UDP_HELPER_H_
#define UDP_HELPER_H_

#include <WiFi.h>
#include "const.h"
#include "eeprom.helper.h"
#include "motor.helper.h"


extern Settings settings;
extern WiFiUDP udp;
extern IPAddress leftSeatIP, rightSeatIP;

void handleUDPmsg();
void sendHello(IPAddress);
String getParam(String, String);
void runMotor(IPAddress, int, String, int);
void stopAllMotors(IPAddress);

#endif
