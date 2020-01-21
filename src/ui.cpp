#include "ui.h"

int uiItem;
int uiItemTop;

void render() {
  display.clearDisplay();
  switch (uiScreen) 
  {
    case UI_MAIN:
      drawMenu("Pick seat", {"Left", "Right", "Options", "Item4", "Item 5", "Item 6", "Item 7", "Item 8"});
      break;

    case UI_MOTOR_SELECT:
      drawMenu("Motor select", {"Motor 1", "Motor 2", "Motor 3", "Motor 4", "Motor 5", "< Back"});
      break;

    case UI_TUNE:
      drawTune();
      break;

    case UI_OPTIONS: 
      drawMenu("Options", {"Connectivity", "Brightness", "Idle-screen", "Restart", "< Back"});
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
  doRender = false;
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
      //drawMenu("Options", {"Connectivity", "Brightness", "Idle-screen", "Restart", "< Back"});
    
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
        case 3:
          ESP.restart();
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
  doProcess = false;
  doRender = true;
}

void drawMenu(String title, const vector<String> &items) {
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
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

void drawTune() {
  display.setTextSize(1);
  display.setTextWrap(true);
  display.setCursor(10, 0);
  display.println("Tune +/-");
  display.print("active seat ");
  display.println(activeSeat);
  display.print("active motor ");
  display.println(activeMotor);
  display.print("rotary ");
  display.println(rotaryVal);
  display.print("executeUntil ");
  display.println(executeUntil);
  display.print("direcrion ");
  display.println(previousDirection);
}






