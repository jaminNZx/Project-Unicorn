/**************************************************************

                            Settings

 **************************************************************/
/*
     Blynk Auth Code
*/
#define AUTH                        "3005c8a9352c45569bbfb2d7f3874053"
/*
   WS28xx Config
*/
#define LED_TYPE                    WS2812B
#define LED_ARRANGE                 GRB
#define LED_NUMBER                  24
#define LED_LIMIT_MILLIAMPS         1500
/*
  Energy Cost
*/
#define ENERGY_PRICE          9.934
/*
   Over The Air Hostname
*/
#define OTA_HOSTNAME                "URBAN-LEAF-MICRO"
/*
   Local Server Settings (uncomment to use local server)
*/
#define SERVER                      IPAddress(192, 168, 1, 2)
/*
   Virtual Pins - Base
*/
#define vPIN_FAN                   V1
#define vPIN_HUE                   V2
#define vPIN_SAT                   V3
#define vPIN_BRI                   V4

#define vPIN_MODE                  V5
#define vPIN_VOLTAGE               V6
#define vPIN_POWER                 V7
#define vPIN_CURRENT               V8
#define vPIN_ENERGY_USED           V9
#define vPIN_ENERGY_COST           V10

#define vPIN_TEMP                  V11
#define vPIN_HUMID                 V12
#define vPIN_HI                    V13

#define vPIN_TOG_POWER             V14
#define vPIN_TOG_TEMP              V15
#define vPIN_HI4                   V16
/*
   Hardware Pins
*/
#define LED_PIN                    13 // GPIO13 / D7 on WeMos/NodeMCU ESP8266
#define FAN_PIN                    12
