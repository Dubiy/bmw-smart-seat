#ifndef UI_H_
#define UI_H_

#include "const.h"
#include <Adafruit_SSD1306.h>
#include <vector>
using std::vector;

extern Adafruit_SSD1306 display;
extern int uiScreen;
extern volatile bool doRender;
extern volatile bool doProcess;
extern volatile int rotaryVal;
extern int activeSeat;
extern int activeMotor;
extern uint32_t executeUntil;
extern unsigned char previousDirection;

void render();
void processMenu();
void drawMenu(String, const vector<String> &items);
void drawTune();

#endif