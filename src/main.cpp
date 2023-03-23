#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>

#include "KioskColors.h"

#define SENSOR_PIN 15
#define LED_CNT 4
#define LED_PIN 13

#define WAIT_TIME 75 

#define SENSOR_ACT   1

#define EFFECT_SOLID_BLUE_FADE 1
#define EFFECT_MEDBLUE_FADE    11
#define EFFECT_STATIC_MEDBLUE  12
#define EFFECT_MEDBLUE_CYCLE   13
#define EFFECT_GREEN_BLINK     2
#define EFFECT_GREEN_FADE      21
#define EFFECT_RED_BLINK       3
#define EFFECT_YELLOW_BLINK    4


bool sensorState = HIGH; 
bool hasRevertedLeds = true; 

const char* ssid = "StallMate-001";
const char* key = "Team_Gundam A+";

CRGB leds[LED_CNT];


// TODO?: 
// Webserver interface... if time allows 

void connectToServer() {

}
/* 
 * 
 *
 * 
 */
void initWebserver() {
  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);

  WebServer server(80);

  WiFi.softAP(ssid, key);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.begin();

  // server.on("/", connectToServer);
}

  


/* 
 * Iterate through RGB LEDs and set 
 * all colors to light blue
 *
 */
void setLightBlue() {
    for (byte i = 0; i < LED_CNT; i++) {
    leds[i] = CRGB::DodgerBlue; 
    FastLED.show();
    delay(25); 
  }
}

/* 
 * void ledInit()
 * 
 * Adds the kiosk's LEDs to the FastLED object.
 * Sets colors to light blue.
 * 
 */
void ledInit() {
  FastLED.addLeds<NEOPIXEL, LED_PIN> (leds, LED_CNT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  setLightBlue();
  Serial.println("LEDs have been initialized successfully");
}

/* 
 * Poll state of kiosk IR sensor
 *
 * @returns true if sensor detected object, false otherwise.  
 */
bool sensorRead() {

  // sensorState = digitalRead(SENSOR_PIN);
  // if (sensorState == LOW) { // IR sensor triggered
  //   return true;
  // }
  return false;
}

byte effectLoop(uint8_t effect_code) {

  byte iterationCnt = 0; 
  bool increment = true; 

  byte colorR; 
  byte colorG; 
  byte colorB;

  switch (effect_code) {

    case EFFECT_SOLID_BLUE_FADE: {
      while (1) {
        Serial.printf("Iteration: %d\n", iterationCnt);
        for (byte i = 0; i < LED_CNT; i++) {
          colorR = 0; 
          colorG = 0;
          // colorG = 41 - (iterationCnt);
          colorB = 221 - (iterationCnt * 5);
          leds[i].setRGB(colorR, colorG, colorB);
        }
        FastLED.show();

        if (sensorRead() == true) {
          Serial.println("Detected sensor input, exiting current effect loop");
          return SENSOR_ACT;
        }

        if (iterationCnt == 0) {
          increment = true; 
        }

        if (iterationCnt > 39) {
          increment = false; 
        }

        switch (increment) {
          case true: 
            iterationCnt++; 
            break; 
          case false: 
            iterationCnt--;
            break;
        }

        delay(WAIT_TIME);
      }
      break;
    }

    case EFFECT_MEDBLUE_FADE: {
      
      while (1) {
        colorR = COLOR_MEDBLUE_R;
        colorG = COLOR_MEDBLUE_G - iterationCnt;
        colorB = COLOR_MEDBLUE_B - (2 * iterationCnt);

        for (byte i = 0; i < LED_CNT; i++) {
          leds[i].setRGB(colorR, colorG, colorB);
        }
        FastLED.show();

        if (sensorRead() == true) {
          return SENSOR_ACT;
        }

        if (iterationCnt == 0) {
          increment = true; 
        }

        if (iterationCnt > 63) {
          increment = false; 
        }

        switch (increment) {

          case true: 
            iterationCnt++;
            break;
          case false: 
            iterationCnt--;
            break;
        }
        delay(WAIT_TIME);
      }
      break;
    }
    
    case EFFECT_MEDBLUE_CYCLE: {

      colorR = COLOR_MEDBLUE_R; 
      colorG = COLOR_MEDBLUE_G;
      colorB = COLOR_MEDBLUE_B;

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }

      while (1) {
        for (byte i = 0; i < LED_CNT; i++) {
          leds[i].setRGB(colorR, colorG, colorB);
          FastLED.show(); 
          leds[i] = CRGB::Black;
          delay(WAIT_TIME * 2);
        }
      }
    }
    
    case EFFECT_GREEN_BLINK: {
      colorR = 0; 
      colorG = 255; 
      colorB = 0; 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(WAIT_TIME * 3); 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      break; 
    }

    case EFFECT_RED_BLINK: {
      colorR = 255; 
      colorG = 0; 
      colorB = 0; 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(WAIT_TIME * 3); 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      break; 
    }

    case EFFECT_YELLOW_BLINK: {
      colorR = 255; 
      colorG = 255; 
      colorB = 0; 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(WAIT_TIME * 3); 

      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB); 
      }
      FastLED.show(); 
      delay(WAIT_TIME * 3); 
      
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      break; 
    }

    case EFFECT_STATIC_MEDBLUE: {

      colorR = COLOR_MEDBLUE_R;
      colorG = COLOR_MEDBLUE_G; 
      colorB = COLOR_MEDBLUE_B;
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB);
      }
      FastLED.show();
      delay(WAIT_TIME);
      break;
    }
  }

  return 1;
}

/* 
 * Arduino Setup function
 * Runs once before loop
 *
 * */
void setup() {
  Serial.begin(115200); 
  ledInit();
  
  effectLoop(EFFECT_GREEN_BLINK);
  initWebserver();
  effectLoop(EFFECT_YELLOW_BLINK);
  effectLoop(EFFECT_RED_BLINK);
  // effectLoop(EFFECT_MEDBLUE_CYCLE);
}

void loop() {

  delay(WAIT_TIME / 3); 
  if (effectLoop(EFFECT_MEDBLUE_FADE) == SENSOR_ACT) {
    effectLoop(EFFECT_GREEN_BLINK);
  }
  // sensorState = digitalRead(SENSOR_PIN);
  // if (sensorRead() == true) {
  //   for (byte i = 0; i < LED_CNT; i++) {
  //     leds[i] = CRGB::LawnGreen; 
  //     FastLED.show(); 
  //   }
  //   hasRevertedLeds = false; 
  //   digitalWrite(LED_BUILTIN, HIGH); 
  //   Serial.println("Detected proximity!");
  //   delay(120); 
  //   digitalWrite(LED_BUILTIN, LOW); 
  // }

  // else {
  //   if (hasRevertedLeds == false) {
  //     setLightBlue(); 
  //   }
  // }

}