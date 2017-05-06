//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#include <FastLED.h>
#include <Adafruit_INA219.h>
#include <ArduinoOTA.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <wifi_credentials.h>
#include "settings.h"

SimpleTimer timer;
Adafruit_INA219 ina219;
CRGB leds[LED_NUMBER];
DHT dht(15, DHT22);

int timer1, timer2, timer3;
int varHue = 200, varSat = 100, varBri = 100, varPattern = 0;
float shuntvoltage, power, busvoltage, loadvoltage, current_mA;
float energy, energyPrice, energyCost, energyPrevious, energyDifference;
float curTemp, curHumid, curHeatIndex, oldTemp, oldHumid, oldHeatIndex;

struct PlantRecipe {
  String  id; // Peas
  String  type; // Micro

  // Germ Stage
  int     germColor; // 0-100% (red)
  int     germHours; // ie: 14 = 14 hrs of light on every 24 hrs
  float   germTemp; // Target Germintation Temp
  float   germHumidity; // Target Germintation Humitdy
  int     germLength; // Number of days until germination is over

  // Grow Stage
  int     growColor;
  int     growHours;
  float   growTemp;
  float   growHumidity;
  int     growLength;
};

PlantRecipe Reciepe_Pea = { "Pea", "micro", 72, 12, 25.12, 70.80, 2, 78, 14, 25.12, 80.00, 12};

void setup() {
  energyPrice = ENERGY_PRICE;
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_LIMIT_MILLIAMPS);
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_ARRANGE>(leds, LED_NUMBER);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
#ifdef SERVER
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, SERVER);
#else
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS);
#endif
  while (Blynk.connect() == false) {}
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.begin();
  ina219.begin();
  dht.begin();
  timer.setInterval(1000, getINA219values);
  timer.setInterval(2000, getDHTvalues);
  timer1 = timer.setInterval(2000, sendINA219values);
  timer.disable(timer1);
}

BLYNK_WRITE(vPIN_TOG_POWER) {
  if (param.asInt()) {
    timer.enable(timer1);
  } else {
    timer.disable(timer1);
  }
}

BLYNK_WRITE(vPIN_TOG_TEMP) {
  if (param.asInt()) {
    timer.enable(timer2);
  } else {
    timer.disable(timer2);
  }
}

void getINA219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000); // V
  power = (current_mA / 1000) * loadvoltage * 1000; // mW
  energy = energy + (power / 1000000);
  energyDifference = energy - energyPrevious;
  energyPrevious = energy;
  energyCost = energyCost + ((energyPrice / 1000 / 100) * energyDifference);
}

void sendINA219values() {
  Blynk.virtualWrite(vPIN_VOLTAGE, String(loadvoltage, 4) + String(" V") );
  if (power > 1000) {
    Blynk.virtualWrite(vPIN_POWER, String((power / 1000), 3) + String(" W") );
  } else {
    Blynk.virtualWrite(vPIN_POWER, String(power, 3) + String(" mW") );
  }
  if (current_mA > 1000) {
    Blynk.virtualWrite(vPIN_CURRENT, String((current_mA / 1000), 3) + String(" A") );
  } else {
    Blynk.virtualWrite(vPIN_CURRENT, String(current_mA, 3) + String(" mA"));
  }
  if (energy > 1000) {
    Blynk.virtualWrite(vPIN_ENERGY_USED, String((energy / 1000), 5) + String(" kWh"));
  } else {
    Blynk.virtualWrite(vPIN_ENERGY_USED, String(energy, 5) + String(" mWh"));
  }
  if (energyCost > 9.999) {
    Blynk.virtualWrite(vPIN_ENERGY_COST, String((energyCost), 7));
  } else {
    Blynk.virtualWrite(vPIN_ENERGY_COST, String((energyCost), 8));
  }
}

void getDHTvalues() {
  curHumid = dht.readHumidity();
  curTemp = dht.readTemperature();
  if (isnan(curHumid) || isnan(curTemp)) return;
  curHeatIndex = dht.computeHeatIndex(curTemp, curHumid, false);
  if(curTemp != oldTemp){
    Blynk.virtualWrite(vPIN_TEMP, curTemp);
    oldTemp = curTemp;
  }
  if(curHumid != oldHumid){
    Blynk.virtualWrite(vPIN_HUMID, curHumid);
    oldHumid = curHumid;
  }
  if(curHeatIndex != oldHeatIndex){
    Blynk.virtualWrite(vPIN_HI, curHeatIndex);
    oldHeatIndex = curHeatIndex;
  }
}

void doPattern() {
  switch (varPattern) {
    case 1:
      fill_solid(leds, LED_NUMBER, CRGB::Blue);
      break;
    case 2:
      fill_solid(leds, LED_NUMBER, CRGB::Blue);
      leds[1] = CRGB::Red;
      leds[3] = CRGB::Red;
      leds[4] = CRGB::Red;
      leds[6] = CRGB::Red;
      leds[17] = CRGB::Red;
      leds[19] = CRGB::Red;
      leds[21] = CRGB::Red;
      leds[23] = CRGB::Red;
      break;
    case 3:
      fill_solid(leds, LED_NUMBER, CRGB::Red);
      leds[0] = CRGB::Blue;
      leds[2] = CRGB::Blue;
      leds[4] = CRGB::Blue;
      leds[6] = CRGB::Blue;
      leds[8] = CRGB::Blue;
      leds[10] = CRGB::Blue;
      leds[12] = CRGB::Blue;
      leds[14] = CRGB::Blue;
      leds[16] = CRGB::Blue;
      leds[18] = CRGB::Blue;
      leds[20] = CRGB::Blue;
      leds[22] = CRGB::Blue;
      break;
    case 4:
      fill_solid(leds, LED_NUMBER, CRGB::Red);
      leds[1] = CRGB::Blue;
      leds[3] = CRGB::Blue;
      leds[4] = CRGB::Blue;
      leds[6] = CRGB::Blue;
      leds[17] = CRGB::Blue;
      leds[19] = CRGB::Blue;
      leds[21] = CRGB::Blue;
      leds[23] = CRGB::Blue;
      break;
    case 5:
      fill_solid(leds, LED_NUMBER, CRGB::Red);
      break;
    default:
      fill_solid(leds, LED_NUMBER, CHSV(map(varHue, 0, 100, 160, 255), varSat, varBri));
      break;
  }
}

BLYNK_WRITE(vPIN_FAN) {
  digitalWrite(FAN_PIN, !param.asInt());
}

BLYNK_WRITE(vPIN_HUE) {
  varHue = param.asInt();
}
BLYNK_WRITE(vPIN_SAT) {
  varSat = param.asInt();
}
BLYNK_WRITE(vPIN_BRI) {
  varBri = param.asInt();
}
BLYNK_WRITE(vPIN_MODE) {
  varPattern = param.asInt();
}


void loop() {
  Blynk.run();
  ArduinoOTA.handle();
  timer.run();
  doPattern();
  FastLED.show();
}



