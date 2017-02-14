//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <wifi_credentials.h>
#include "settings.h"

SimpleTimer timer;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
#if defined(USE_LOCAL_SERVER)
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, SERVER);
#else
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, );
#endif
  while (Blynk.connect() == false) {}
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.begin();
}

void loop() {
  Blynk.run();
  ArduinoOTA.handle();
  timer.run();
}
