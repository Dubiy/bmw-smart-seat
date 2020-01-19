#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Rotary.h>
#include <vector>

using std::vector;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define ROTARY_A 4
#define ROTARY_B 5
#define ROTARY_BUTTON 2

#define UI_MAIN 0
#define UI_MOTOR_SELECT 1
#define UI_TUNE 2
#define UI_OPTIONS 3
#define UI_OPT_CONNECT 4
#define UI_OPT_IDLE 5

#define LEFT_SEAT 1
#define RIGHT_SEAT 2

#define MOTOR_ANGLE 1
#define MOTOR_FORWARD 2
#define MOTOR_HEAD 3
#define MOTOR_HZ1 4
#define MOTOR_HZ2 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Rotary rotary = Rotary(ROTARY_A, ROTARY_B);
 
void testdrawchar(int);
void onButtonPress();
void render();
void drawMenu(String, const vector<String> &items);
void processMenu();
void drawTune();

volatile bool doProcess = false;

volatile int uiScreen = UI_MAIN;
int uiItem = 0;
int uiItemTop = 0;
int renderInterval = 1000000;
int activeSeat = 0;
int activeMotor = 0;

volatile int rotaryVal = 0;

volatile int menuItem = 0;

uint32_t lastService = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ROTARY_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON), onButtonPress, RISING);

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  rotary.begin(true);





}
 
void loop() {
  unsigned char result = rotary.process(); // rewrite on iterrupts

  if (lastService + renderInterval < micros()) {
    lastService = micros();                
    render();
  }


  if (result) {
    rotaryVal += result == DIR_CW ? 1 : -1;
    
    testdrawchar(rotaryVal);
    Serial.println(result == DIR_CW ? "Right" : "Left");
  }

  if (doProcess) {
    doProcess = false;
    processMenu();
  }
}



void render() {
  display.clearDisplay();
  switch (uiScreen) 
  {
    case UI_MAIN:
      drawMenu("Select seat", {"Left", "Right", "Options", "Item4", "Item 5", "Item 6", "Item 7", "Item 8"});
      break;

    case UI_MOTOR_SELECT:
      drawMenu("Motor select", {"Motor 1", "Motor 2", "Motor 3", "Motor 4", "Motor 5", "< Back"});
      break;

    case UI_TUNE:
      drawTune();
      break;

    case UI_OPTIONS: 
      drawMenu("Options", {"Connectivity", "Brightness", "Idle-screen", "< Back"});
      break;

    case UI_OPT_CONNECT:
      drawMenu("Connectivity", {"Off", "Wifi", "Bluetooth", "< Back"});
      break;

    case UI_OPT_IDLE: 
      drawMenu("Idle", {"Off", "Seat-selection", "Time", "Time+Voltage", "Other...", "< Back"});
      break;

    default:
      drawMenu("Undefined", {"< Back"});
      break;
  }
  display.display();
}

void drawMenu(String title, const vector<String> &items) {
  display.setTextSize(2);
  display.setTextWrap(false);
  display.setCursor(10, 0);
  display.println(title);
  display.drawLine(0, 15, SCREEN_WIDTH, 15, SSD1306_WHITE);

  int itemsSize = items.size();
  //activeItem must be between [0 and items.size()-1]
  rotaryVal = rotaryVal < 0 ? 0 : ( rotaryVal < itemsSize ? rotaryVal : itemsSize - 1 );

  // define part of menu to render
  // if this is not begin or end - cursor in the middle.
  int startItem = 0;
  if (rotaryVal > 1) {
    startItem = (rotaryVal < itemsSize - 2) ? rotaryVal - 1 : itemsSize - 3;
  }
  int stopItem = startItem + 3 >= itemsSize ? itemsSize - 1 : startItem + 3;

  for (int itemId = startItem, i = 0; itemId <= stopItem; itemId++, i++) {
    display.setCursor(15, i*16 + 17);
    if (itemId == rotaryVal) {
      display.fillRect(5, i*16+16, 123, 16, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.println(items[itemId]);
  } 
}

void processMenu() {
  switch (uiScreen) 
  {
    case UI_MAIN:
      // drawMenu("Select seat", {"Left", "Right", "Options"});

      uiItemTop = uiItem = rotaryVal;
      rotaryVal = 0;

      switch (uiItem)
      {
        case 0:
          uiScreen = UI_MOTOR_SELECT;
          activeSeat = LEFT_SEAT;
          break;
        case 1:
          uiScreen = UI_MOTOR_SELECT;
          activeSeat = RIGHT_SEAT;
          break;
        case 2: 
          uiScreen = UI_OPTIONS;
          break;
      } 
      break;

    case UI_MOTOR_SELECT:
      //drawMenu("Motor select", {"Motor 1", "Motor 2", "Motor 3", "Motor 4", "Motor 5", "< Back"});

      uiItem = rotaryVal;
      rotaryVal = 0;
      
      switch (uiItem)
      {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
          uiScreen = UI_TUNE;
          activeMotor = uiItem;
          /* code */
          break;
        
        default: 
          uiScreen = UI_MAIN;
          rotaryVal = uiItemTop;
          break;
      } 
      break;

    case UI_TUNE:
      uiScreen = UI_MOTOR_SELECT;
      rotaryVal = uiItem;


      break;

    case UI_OPTIONS: 
      //drawMenu("Options", {"Connectivity", "Brightness", "Idle-screen", "< Back"});
    
      uiItemTop = uiItem = rotaryVal;
      rotaryVal = 0;

      switch (uiItem)
      {
        case 0:
          uiScreen = UI_OPT_CONNECT;
          break;

        case 1: 
          uiScreen = UI_OPTIONS;
          break;

        case 2:
          uiScreen = UI_OPT_IDLE;
          /* code */
          break;
        default:
          uiScreen = UI_MAIN;
          rotaryVal = 2;
          break; 
      } 
      

      break;

    case UI_OPT_CONNECT:
      //drawMenu("Connectivity", {"Off", "Wifi", "Bluetooth", "< Back"});

      uiItem = rotaryVal;
      rotaryVal = 0;
      
      switch (uiItem)
      {
        case 0:
        case 1:
        case 2:
          uiScreen = UI_OPT_CONNECT;
          /* code */
          break;
        
        default: 
          uiScreen = UI_OPTIONS;
          rotaryVal = uiItemTop;
          break;
      } 
      break;


    case UI_OPT_IDLE: 
      // drawMenu("Idle", {"Off", "Seat-selection", "Time", "Time+Voltage", "Other...", "< Back"});
      uiItem = rotaryVal;
      rotaryVal = 0;
      
      switch (uiItem)
      {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
          uiScreen = UI_OPT_IDLE;
          /* code */
          break;
        
        default: 
          uiScreen = UI_OPTIONS;
          rotaryVal = uiItemTop;
          break;
      } 
      break;


    default:
      // drawMenu("Undefined", {"< Back"});

      uiScreen = UI_MAIN;
      rotaryVal = 0;
      break;
  }
}

void drawTune() {
  display.setTextSize(1);
  display.setTextWrap(true);
  display.setCursor(10, 0);
  display.println("Tune +/-");
  display.print("active seat ");
  display.println(activeSeat);
  display.print("active motor ");
  display.println(activeMotor);
  
}

void testdrawchar(int number) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(number);
  display.display();
}

void onButtonPress() {
  // rotaryVal = -rotaryVal;
  doProcess = true;
}

