#include <Arduino.h>
#include <FastLED.h>

#define SENSOR_PIN 15
#define LED_CNT 5
#define LED_PIN 13

bool sensorState = HIGH; 
bool hasRevertedLeds = true; 

CRGB leds[LED_CNT];

void setLightBlue() {
    for (byte i = 0; i < LED_CNT; i++) {
    leds[i] = CRGB::DodgerBlue; 
    FastLED.show();
    delay(200); 
  }
}

void setup() {

  Serial.begin(115200); 
  FastLED.addLeds<NEOPIXEL, LED_PIN> (leds, LED_CNT);

  setLightBlue();
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
}

void loop() {

  delay(150); 
  sensorState = digitalRead(SENSOR_PIN);
  if (sensorState == LOW) {
    for (byte i = 0; i < LED_CNT; i++) {
      leds[i] = CRGB::LawnGreen; 
      FastLED.show(); 
    }
    hasRevertedLeds = false; 
    digitalWrite(LED_BUILTIN, HIGH); 
    Serial.println("Detected proximity!");
    delay(120); 
    digitalWrite(LED_BUILTIN, LOW); 
  }

  else {
    if (hasRevertedLeds == false) {
      setLightBlue(); 
    }
  }

  // put your main code here, to run repeatedly:
}