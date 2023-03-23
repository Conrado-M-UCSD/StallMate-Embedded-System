#include <Arduino.h>
#include <FastLED.h>

#define SENSOR_PIN 15
#define LED_CNT 4
#define LED_PIN 13

#define EFFECT_BLUE_TO_WHITE_FADE 1
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

  switch (effect_code) {
    case EFFECT_BLUE_TO_WHITE_FADE:
      while (1) {
        for (byte i = 0; i < LED_CNT; i++) {
          leds[i] = CRGB::DodgerBlue;
        }
        FastLED.show();

        if (sensorRead() == true) {
          break;
        }
      }
      break;

    case EFFECT_GREEN_BLINK: 
      break; 
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

  effectLoop(EFFECT_BLUE_TO_WHITE_FADE);
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