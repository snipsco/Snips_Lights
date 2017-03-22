#include "Adafruit_NeoPixel.h"
#include "Snips_Lights.h"

#define PIXELS_WHITE false
#define PIXELS 24
#define PIN 6

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_RGBW    Pixels are wired for GRBW bitstream (NeoPixel GRBW products)
#if PIXELS_WHITE
  #define PIXEL_TYPE NEO_GRBW + NEO_KHZ800
#else
  #define PIXEL_TYPE NEO_GRB + NEO_KHZ800
#endif

Snips_Lights *lights;

void setup() {
  Serial.begin(9600);
  lights = new Snips_Lights(PIXELS, PIN, PIXEL_TYPE);
  lights->getPixels()->setBrightness(40);
  lights->setState(SLStateWakingUp);
}

//  0s: wake up
//  5s: listen
// 10s: load
// 15s: yes
// 20s: error
// 25s: shut down
// 30s: restart

SLTimeInterval startTime = millis();

void loop() {
  SLState state = lights->getState();
  SLTimeInterval offset = millis() - startTime;
  if (state != SLStateListening && 5000 <= offset && offset < 10000) {
    Serial.println("Set listening");
    lights->setState(SLStateListening);
  } else if (state != SLStateLoading && 10000 <= offset && offset < 15000) {
    Serial.println("Set loading");
    lights->setState(SLStateLoading);
  } else if (state != SLStateYes && 15000 <= offset && offset < 20000) {
    Serial.println("Set yes");
    lights->setState(SLStateYes);
  } else if (state != SLStateError && 20000 <= offset && offset < 25000) {
    Serial.println("Set error");
    lights->setState(SLStateError);
  } else if (state != SLStateShuttingDown && state != SLStateNone && 25000 <= offset) {
    Serial.println("Set shutting down");
    lights->setState(SLStateShuttingDown);
  } else if (30000 <= offset) {
    Serial.println("\n---\n");
    lights->setState(SLStateWakingUp);
    startTime = millis();
  }
  lights->step();
}