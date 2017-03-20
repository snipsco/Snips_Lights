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

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS, PIN, PIXEL_TYPE);
Snips_Lights lights = Snips_Lights(&pixels);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(10);
  lights.setState(SLStateWakingUp);
}

//  0s: wake up
//  5s: listen
// 10s: load
// 15s: yes
// 20s: error
// 25s: shut down

SLTimeInterval startTime = millis();
SLState state = SLStateNone;

void loop() {
  SLTimeInterval offset = millis() - startTime;
  if (state != SLStateListening && 5000 <= offset && offset < 10000) {
    state = SLStateListening;
    lights.setState(state);
  } else if (state != SLStateLoading && 10000 <= offset && offset < 15000) {
    state = SLStateLoading;
    lights.setState(state);
  } else if (state != SLStateYes && 15000 <= offset && offset < 20000) {
    state = SLStateYes;
    lights.setState(state);
  } else if (state != SLStateError && 20000 <= offset && offset < 25000) {
    state = SLStateError;
    lights.setState(state);
  } else if (state != SLStateShuttingDown && 25000 <= offset) {
    state = SLStateShuttingDown;
    lights.setState(state);
  }
  lights.step();
}
