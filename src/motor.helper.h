#ifndef MOTOR_HELPER_H_
#define MOTOR_HELPER_H_

#include <Arduino.h>
#include <Rotary.h>
#include "const.h"

extern unsigned char previousDirection;
extern int activeMotor;
extern uint32_t executeUntil;

void motorStart(int, String, int);
void motorStopAll();
void motorInit();

#endif
