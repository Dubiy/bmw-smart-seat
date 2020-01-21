#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Rotary.h>

#include "const.h"

#include "ui.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //extern
Rotary rotary = Rotary(ROTARY_DATA_PIN, ROTARY_CLK_PIN); //extern
 
void onButtonPress();
void onRotate();

volatile bool doProcess = false; 
volatile bool doRender = false; //extern
volatile int rotaryVal = 0; //extern
volatile unsigned char rotateResult = 0;
unsigned char previousDirection = 0; //extern

int uiScreen = UI_MAIN; //extern
int activeSeat = 0; //extern
int activeMotor = 0; //extern
int renderInterval = 1000000;
uint32_t lastRender = 0;
uint32_t executeUntil = 0; //extern



void setup() {
  Serial.begin(9600);
  
  pinMode(ROTARY_BUTTON_PIN, INPUT_PULLUP);
  rotary.begin(true);
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON_PIN), onButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_DATA_PIN), onRotate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_CLK_PIN), onRotate, CHANGE);

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);

  render();
}
 
void loop() {
  if (doRender || lastRender + renderInterval < micros()) {
    render();
    lastRender = micros();
  }
  
  if (rotateResult && uiScreen == UI_TUNE) {
    if (previousDirection != rotateResult && previousDirection) {
      executeUntil = 0;
    } else {
      executeUntil = millis() + 1000;
    }
    
    doRender = true;
    previousDirection = rotateResult;
    rotateResult = 0;
  }

  if (executeUntil) {
    if (executeUntil > millis()) {
      // rotate motor
    } else {
      // stop
      executeUntil = 0;
    }
  } 


  if (doProcess) {
    processMenu();
  }
}








void onButtonPress() {
  doProcess = true;
}

void onRotate() {
  rotateResult = rotary.process();
  if (rotateResult) {
    rotaryVal += rotateResult == DIR_CW ? 1 : -1;
    doRender = true;
  }
}