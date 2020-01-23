#include "eeprom.helper.h"
#include "const.h"

int _eepromStart = 0;

byte CRC(const String data) {
  byte len = data.length();
  byte crc = 0x00;
  
  while (len--) {
    byte extract = data.charAt(len);
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

void setEEPROMString(int start, int len, String string) {
  int si = 0;
  Serial.print("Save to EEPROM [");
  Serial.print(start);
  Serial.print(", ");
  Serial.print(len);
  Serial.print("] - ");
  Serial.println(string);
  for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
    char c;
    if (si < string.length()) {
      c = string[si];

    } else {
      c = 0;
    }
    EEPROM.write(i, c);
    si++;
  }

}

String getEEPROMString(int start, int len) {
  String string = "";
  for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
    //DEBUG_PRINT(i);
    byte val = EEPROM.read(i);
    if (val == 0) {
      return string;
    }
    string += char(val);
  }
  return string;
}

Settings getSettings() {
  Settings settings;
  settings.ssid = getEEPROMString(settings.ssid_offset, settings.ssid_length);
  settings.pass = getEEPROMString(settings.pass_offset, settings.pass_length);
  settings.secret = getEEPROMString(settings.secret_offset, settings.secret_length);
  settings.title = getEEPROMString(settings.title_offset, settings.title_length);
  // settings.brightness = getEEPROMString(settings.brightness_offset, settings.brightness_length);
  settings.crc = getEEPROMString(settings.crc_offset, settings.crc_length);

  Serial.println("==== SETTINGS ====");
  Serial.println("==================");

  Serial.print("ssid: ");
  Serial.println(settings.ssid);

  Serial.print("pass: ");
  Serial.println(settings.pass);

  Serial.print("secret: ");
  Serial.println(settings.secret);

  Serial.print("title: ");
  Serial.println(settings.title);

  // Serial.print("brightness: ");
  // Serial.println(settings.brightness.toInt());

  Serial.print("crc: ");
  Serial.println(settings.crc.toInt());

  Serial.println("==================");


  if (
    settings.ssid + settings.pass + settings.secret + settings.title == "" ||
    settings.crc.toInt() != CRC(settings.ssid + settings.pass + settings.secret + settings.title /*+ settings.brightness*/)
  ) {
    Serial.println("Settings CRC not match. load default settings");
    settings.ssid = DEVICE_NAME + WiFi.softAPmacAddress();
    settings.pass = "";
    settings.title = DEVICE_NAME;
    settings.secret = "";
    // settings.brightness = "50";
    settings.crc = "-1";
  }
  return settings;
}

void setSettings(Settings &settings) {
  settings.crc = String(CRC(settings.ssid + settings.pass + settings.secret + settings.title /* + settings.brightness*/));
  setEEPROMString(settings.ssid_offset,       settings.ssid_length,       settings.ssid);
  setEEPROMString(settings.pass_offset,       settings.pass_length,       settings.pass);
  setEEPROMString(settings.secret_offset,     settings.secret_length,     settings.secret);
  setEEPROMString(settings.title_offset,      settings.title_length,      settings.title);
  // setEEPROMString(settings.brightness_offset, settings.brightness_length, settings.brightness);
  setEEPROMString(settings.crc_offset,        settings.crc_length,        settings.crc);
  Serial.println(EEPROM.commit() ? "EEPROM saved successfully" : "EEPROM commit problem")  ;
}

String urldecode(const char *src) {
  String decoded = "";
  char a, b;
  while (*src) {
    if ((*src == '%') &&
        ((a = src[1]) && (b = src[2])) &&
        (isxdigit(a) && isxdigit(b))) {
      if (a >= 'a')
        a -= 'a' - 'A';
      if (a >= 'A')
        a -= ('A' - 10);
      else
        a -= '0';
      if (b >= 'a')
        b -= 'a' - 'A';
      if (b >= 'A')
        b -= ('A' - 10);
      else
        b -= '0';

      decoded += char(16 * a + b);
      src += 3;
    } else if (*src == '+') {
      decoded += ' ';
      *src++;
    } else {
      decoded += *src;
      *src++;
    }
  }
  decoded += '\0';

  return decoded;
}
