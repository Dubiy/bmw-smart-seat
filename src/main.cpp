#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Rotary.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include "eeprom.helper.h"
#include "webserver.helper.h"
#include "const.h"
#include "ui.h"
#include "wifi.helper.h"
#include "udp.helper.h"
#include <queue>


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //extern
Rotary rotary = Rotary(ROTARY_DATA_PIN, ROTARY_CLK_PIN); //extern
WebServer webServer(80); //extern
DNSServer dnsServer; //extern   
WiFiUDP udp; //extern

void onButtonPress();
void onRotate();

volatile bool doProcess = false; 
volatile bool doRender = false; //extern
volatile int rotaryVal = 0; //extern
volatile unsigned char rotateResult = 0;
unsigned char previousDirection = 0; //extern

int uiScreen = UI_DEBUG; //UI_MAIN; //extern
int activeSeat = 0; //extern
int activeMotor = 0; //extern
int renderInterval = 1000;
uint32_t lastService = 0;
int serviceCount = 0;
uint32_t executeUntil = 0; //extern
Settings settings; //extern
queue<struct task> q; //extern
IPAddress leftSeatIP, rightSeatIP;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(160);
  
  pinMode(ROTARY_BUTTON_PIN, INPUT_PULLUP);
  rotary.begin(true);
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON_PIN), onButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_DATA_PIN), onRotate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_CLK_PIN), onRotate, CHANGE);

  #if ROLE_CLIENT
    motorInit();

  #endif

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);

  settings = getSettings();
  WiFi.persistent(false);
  int status = -1;
  if (settings.crc.toInt() >= 0) {
    //try to connect to Wifi
    if (ROLE_SERVER) {
      Serial.println("ROLE_SERVER");
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(settings.apIP, settings.apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(settings.ssid.c_str(), settings.pass.c_str(), 1, 0, 4);
      Serial.print("Soft-AP IP address = ");
      Serial.println(WiFi.softAPIP());
    } else {
      Serial.println("ROLE_CLIENT");
      WiFi.mode(WIFI_STA);
      WiFi.begin(settings.ssid.c_str(), settings.pass.c_str());
      status = WiFi.waitForConnectResult();
    }
  }

  if (
    ROLE_SERVER && !WiFi.softAPIP() ||
    ROLE_CLIENT && status != WL_CONNECTED
  ) {
    // setup module as AP
    Serial.println("Connection Failed. Starting AP...");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    
    

    
    
    // WiFi.softAP(settings.ssid.c_str());
    WiFi.softAPConfig(settings.apIP, settings.apIP, IPAddress(255, 255, 255, 0));

    WiFi.softAP(settings.ssid.c_str(), settings.pass.c_str(), 1, 0, 4);
    uiScreen = UI_WIFI_INFO;
  }

  




  WiFi.onEvent(wifiEventHandler);

  initWebserver();

  udp.begin(settings.udpPort);


  //start captive portal
  dnsServer.start(53, "*", settings.apIP); // Capture DNS requests on port 53. if DNSServer is started with "*" for domain name, it will reply with provided IP to all DNS request

  render();
}
 



void loop() {
  uint32_t now = millis();

  dnsServer.processNextRequest();
  webServer.handleClient();

  if (udp.parsePacket()) {
    handleUDPmsg();
  }

  if (doRender) {
    render();
  }


  if (lastService + renderInterval < now) {
    render();

    if (q.size()) {
      for (int i = 0; i < q.size(); i++) {
        auto item = q.front();
        q.pop();
        if (item.executeAt < now) {
          item.f();
          // q.push({ millis() + 5000, [] {Serial.println("Even more fun 5000"); } });
        } else {
          q.push(item);
        }
      }
      
    }

    #if ROLE_CLIENT
      if (WiFi.getMode() == WIFI_MODE_AP && serviceCount > 20) {
        //scan, if server is available
        if (WiFi.softAPgetStationNum() == 0) {
          ESP.restart();
        }
      }
    #endif

    // Serial.print(".");
    lastService = now;
    serviceCount++;
  }
  
  if (rotateResult && uiScreen == UI_TUNE) {

    IPAddress seatIp;
    Serial.print("TUNE: ");

    
    if (activeSeat == LEFT_SEAT) {
      seatIp = leftSeatIP;
      Serial.print("left, ");
    }
    if (activeSeat == RIGHT_SEAT) {
      seatIp = rightSeatIP;
      Serial.print("right, ");
    }
    
    if (previousDirection != rotateResult && previousDirection) {
      executeUntil = 0;
      Serial.print("stop ");
      Serial.print(seatIp);
      if (seatIp != 0) {
        stopAllMotors(seatIp);
      }
    } else {
      int duration = 1000;
      executeUntil = now + duration;

      
      
      if (seatIp != 0) {
        runMotor(seatIp, activeMotor, rotateResult == DIR_CW ? "CW" : "CCW", duration);
      }


// activeMotor
// rotateResult
      

      
    }
    
    doRender = true;
    previousDirection = rotateResult;
    rotateResult = 0;
  }

  if (executeUntil && executeUntil < now) {
      stopAllMotors(leftSeatIP);
      stopAllMotors(rightSeatIP);
      executeUntil = 0;
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