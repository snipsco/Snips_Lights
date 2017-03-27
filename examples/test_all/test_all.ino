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
  lights->setState(SLStateWakingUp);
}

//  0s: wake up
//  5s: listen
// 10s: error
// 15s: shut down
// 20s: restart

SLTimeInterval startTime = millis();

void loop() {
  SLState state = lights->getState();
  SLTimeInterval offset = millis() - startTime;
  if (state != SLStateListening && 5000 <= offset && offset < 10000) {
    Serial.println("Set listening");
    lights->setState(SLStateListening);
  } else if (state != SLStateError && state != SLStateStandby && 10000 <= offset && offset < 15000) {
    Serial.println("Set error");
    lights->setState(SLStateError);
  } else if (state != SLStateNone && 15000 <= offset) {
    Serial.println("Set none");
    lights->setState(SLStateNone);
  } else if (20000 <= offset) {
    Serial.println("\n---\n");
    lights->setState(SLStateWakingUp);
    startTime = millis();
  }
  lights->step();
}
