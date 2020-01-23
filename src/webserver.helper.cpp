#include "webserver.helper.h"

void initWebserver() {
  webServer.onNotFound([]() { // replay to all requests with same HTML
    Serial.println("GET /404 (index)");
    webServer.send(200, "text/html", indexHtml());
  });

  webServer.on("/", HTTP_POST, []() {
    Serial.println("POST /");
    webServer.send(200, "text/html", indexHtml());
    // BLINKING = !BLINKING;
  });

  webServer.on("/", []() {
    Serial.println("GET /");
    webServer.send(200, "text/html", indexHtml());
  });

  webServer.on("/config", HTTP_POST, []() {
    Serial.println("POST /config");
    webServer.send(200, "text/html", configHtml(webServer, settings, true));

    delay(50);
    ESP.restart();
  });

  webServer.on("/config", HTTP_GET, []() {
    Serial.println("GET /config");
    webServer.send(200, "text/html", configHtml(webServer, settings, false));
  });

  webServer.begin();

}