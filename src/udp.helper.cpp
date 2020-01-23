#include "udp.helper.h"

void handleUDPmsg() {
  char packetBuffer[255];
  Serial.println("Received packet");
  Serial.print("From ");
  IPAddress remoteIp = udp.remoteIP();
  Serial.print(remoteIp);
  Serial.print(", port ");
  Serial.println(udp.remotePort());
  int len = udp.read(packetBuffer, 255);
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  String packet = String(packetBuffer);

  Serial.print("Contents: ");
  Serial.println(packetBuffer);

  

  if (packet == CMD_HELLO) {
    udp.beginPacket(remoteIp, udp.remotePort());   
    udp.print((">" + String(CMD_HELLO) + "&ssid=" + settings.ssid+ "&device=" + DEVICE_NAME).c_str());
    udp.endPacket();
  }
  if (packet.startsWith(">" + String(CMD_HELLO))) {
      Serial.println("packet.startsWith HELLO");
      Serial.println(getParam(packet, "device"));
      if (getParam(packet, "device") == LEFT_SEAT_TITLE) {
        leftSeatIP = remoteIp;
        Serial.println("Register left seat");
      }
      if (getParam(packet, "device") == RIGHT_SEAT_TITLE) {
        rightSeatIP = remoteIp;
        Serial.println("Register right seat");
      }
  }
  if (packet.startsWith(CMD_RUN)) {
      int motorId = getParam(packet, "motorId").toInt();
      int duration = getParam(packet, "duration").toInt();
      String direction = getParam(packet, "direction");
      Serial.print("RUN! Motor: ");
      Serial.print(motorId);
      Serial.print(", direction: ");
      Serial.print(direction);
      Serial.print(", duration: ");
      Serial.println(duration);
      
      motorStart(motorId, direction, duration);
      // do execute
      // do response
  }

  if (packet.startsWith(CMD_STOP)) {
      // do motor stop
      // do response
      motorStopAll();
  }


  if (packet == "stop") {
    // BLINKING = false;
  }

//   udp.beginPacket(remoteIp, udp.remotePort());   
//   udp.print(("|SmartSeat|Left|" + settings.title).c_str());
//   udp.endPacket();
}

String getParam(String query, String name) {
  // >run&motorId=3&duration=1000
  int pos = query.indexOf("&" + name + "=");
  if (pos >= 0) {

    String valueStart = query.substring(pos + 2 + name.length());
    pos = valueStart.indexOf("&");
    if (pos >= 0) {
      return valueStart.substring(0, pos);
    }
    return valueStart;
  }
  return "";
}

void sendHello(IPAddress ip) {
    Serial.print("Send Hello CMD to ");
    Serial.print(ip);
    Serial.print(":");
    Serial.println(settings.udpPort);

    udp.beginPacket(ip, settings.udpPort);   
    udp.print(CMD_HELLO);
    Serial.println(udp.endPacket() ? "ok" : "fail");
}

void runMotor(IPAddress ip, int motorId, String direction, int duration) {
    if (ip == leftSeatIP) {
      Serial.print("Run motor. LEFT seat. Motor: ");
    } else if (ip == rightSeatIP) {
      Serial.print("Run motor. RIGHT seat. Motor: ");
    } else {
      Serial.println("UNKNOWN IP. BREAK");
      return;
    }
    Serial.print(motorId);
    Serial.print(", direction: ");
    Serial.print(direction); 
    Serial.print(", duration: ");
    Serial.println(duration);

    Serial.print(ip);
    Serial.print(":");
    Serial.println(settings.udpPort);

    udp.beginPacket(ip, settings.udpPort);   
    udp.print(String(CMD_RUN) + "&motorId=" + motorId + "&direction=" + direction + "&duration=" + duration);
    Serial.println(udp.endPacket() ? "ok" : "fail");
}

void stopAllMotors(IPAddress ip) {
  if (ip != 0) {
    Serial.println("stopAllMotors");
    Serial.print(ip);
    Serial.print(":");
    Serial.println(settings.udpPort);

    udp.beginPacket(ip, settings.udpPort);   
    udp.print(CMD_STOP);
    Serial.println(udp.endPacket() ? "ok" : "fail");
  }
    
}