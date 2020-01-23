#include "motor.helper.h"

void motorStart(int motorId, String direction, int duration) {
  // https://docs.google.com/document/d/1hGR1vgYvU2xrwMrx8YlcpkRuGoexXucTEZ_KvWTLZuM/edit

  Serial.println(direction);
  Serial.println(direction == "CW");
  Serial.println( direction == "CW" ? DIR_CW : DIR_CCW);
  activeMotor = motorId;
  previousDirection = direction == "CW" ? DIR_CW : DIR_CCW;
  executeUntil = millis() + duration; 

  digitalWrite(RELAY1_PIN, RELAY_OFF);
  digitalWrite(RELAY2_PIN, RELAY_OFF);
  digitalWrite(RELAY3_PIN, RELAY_OFF);
  digitalWrite(RELAY4_PIN, RELAY_OFF);
  digitalWrite(RELAY5_PIN, RELAY_OFF);
  digitalWrite(RELAY6_PIN, RELAY_OFF);
  digitalWrite(RELAY7_PIN, RELAY_OFF);
  digitalWrite(RELAY8_PIN, RELAY_OFF);

  switch (motorId) 
  {
    case MOTOR_ANGLE:
        // 3 (1/2)
        digitalWrite(RELAY3_PIN, RELAY_ON);
        if (previousDirection == DIR_CW) {
          digitalWrite(RELAY1_PIN, RELAY_ON);
        } else {
          digitalWrite(RELAY2_PIN, RELAY_ON);
        }
    break;
    case MOTOR_FORWARD:
        // (1/2)
        if (previousDirection == DIR_CW) {
          digitalWrite(RELAY1_PIN, RELAY_ON);
        } else {
          digitalWrite(RELAY2_PIN, RELAY_ON);
        }

    break;
    case MOTOR_HEAD:
        // 6 (4/5)
        digitalWrite(RELAY6_PIN, RELAY_ON);
        if (previousDirection == DIR_CW) {
          digitalWrite(RELAY4_PIN, RELAY_ON);
        } else {
          digitalWrite(RELAY5_PIN, RELAY_ON);
        }
    break;
    case MOTOR_HZ1:
        // (4/5)
        if (previousDirection == DIR_CW) {
          digitalWrite(RELAY4_PIN, RELAY_ON);
        } else {
          digitalWrite(RELAY5_PIN, RELAY_ON);
        }

    break;
    case MOTOR_HZ2:
        // (7/8)
        if (previousDirection == DIR_CW) {
          digitalWrite(RELAY7_PIN, RELAY_ON);
        } else {
          digitalWrite(RELAY8_PIN, RELAY_ON);
        }
    break;
  default:
    break;
  }
}

void motorStopAll() {
  Serial.println("Stop all motors");
  digitalWrite(RELAY1_PIN, RELAY_OFF);
  digitalWrite(RELAY2_PIN, RELAY_OFF);
  digitalWrite(RELAY3_PIN, RELAY_OFF);
  digitalWrite(RELAY4_PIN, RELAY_OFF);
  digitalWrite(RELAY5_PIN, RELAY_OFF);
  digitalWrite(RELAY6_PIN, RELAY_OFF);
  digitalWrite(RELAY7_PIN, RELAY_OFF);
  digitalWrite(RELAY8_PIN, RELAY_OFF);
}

void motorInit() {
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, RELAY_OFF);

  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, RELAY_OFF);

  pinMode(RELAY3_PIN, OUTPUT);
  digitalWrite(RELAY3_PIN, RELAY_OFF);

  pinMode(RELAY4_PIN, OUTPUT);
  digitalWrite(RELAY4_PIN, RELAY_OFF);

  pinMode(RELAY5_PIN, OUTPUT);
  digitalWrite(RELAY5_PIN, RELAY_OFF);

  pinMode(RELAY6_PIN, OUTPUT);
  digitalWrite(RELAY6_PIN, RELAY_OFF);

  pinMode(RELAY7_PIN, OUTPUT);
  digitalWrite(RELAY7_PIN, RELAY_OFF);

  pinMode(RELAY8_PIN, OUTPUT);
  digitalWrite(RELAY8_PIN, RELAY_OFF);
}