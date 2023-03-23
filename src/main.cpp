#include <Arduino.h>
#include <FastLED.h>

#define SENSOR_PIN 15
#define LED_CNT 4
#define LED_PIN 13


#define WAIT_TIME 75 

#define EFFECT_BLUE_FADE 1
#define EFFECT_STATIC_BLUE 11
#define EFFECT_GREEN_BLINK 2


bool sensorState = HIGH; 
bool hasRevertedLeds = true; 

CRGB leds[LED_CNT];

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

    case EFFECT_BLUE_FADE: {
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
          break;
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

    case EFFECT_GREEN_BLINK: 
      break; 

    case EFFECT_STATIC_BLUE: {
      colorR = 34; 
      colorG = 70; 
      colorB = 168;
      for (byte i = 0; i < LED_CNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB);
      }
      FastLED.show();
      delay(WAIT_TIME);
      break;
    }
  }
}

/* 
 * Arduino Setup function
 * Runs once before loop
 *
 * */
void setup() {
  Serial.begin(115200); 
  ledInit();

}

void loop() {

  delay(69); 
  effectLoop(EFFECT_STATIC_BLUE);
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