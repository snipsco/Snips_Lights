#include "Adafruit_NeoPixel.h"
#include "Snips_Lights.h"

#define PIXELS_WHITE true
#define PIXELS 24
#define PIN 6

#if PIXELS_WHITE
  #define WHITE_COLOR(pixels, value) pixels->Color(0, 0, 0, value)
#else
  #define WHITE_COLOR(pixels, value) pixels->Color(value, value, value)
#endif

Snips_Lights *lights;

void setup() {
  Serial.begin(9600);
  lights = new Snips_Lights(PIXELS, PIN, NEO_KHZ800 + NEO_GRBW);
  lights->setPrimaryColor(WHITE_COLOR(lights->getPixels(), 255));
  lights->getPixels()->setBrightness(40);
}

void loop() {
  if (Serial.available()) {
    int next = Serial.read();
    SLState newState = SLStateNone;
    UPDATE_STATE(&newState, next);
    lights->setState(newState);
  }
  lights->step();
}
