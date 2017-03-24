#include "Adafruit_NeoPixel.h"
#include "Snips_Lights.h"

#define PIXELS_WHITE false
#define PIXELS 24
#define PIN 6

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_RGBW    Pixels are wired for GRBW bitstream (NeoPixel GRBW products)
#if PIXELS_WHITE
  #define PIXEL_TYPE NEO_GRBW + NEO_KHZ800
  #define WHITE_COLOR(pixels, value) pixels->Color(0, 0, 0, value)
#else
  #define PIXEL_TYPE NEO_GRB + NEO_KHZ800
  #define WHITE_COLOR(pixels, value) pixels->Color(value, value, value)
#endif

Snips_Lights *lights;

void setup() {
  Serial.begin(9600);
  lights = new Snips_Lights(PIXELS, PIN, PIXEL_TYPE);
  lights->setPrimaryColor(WHITE_COLOR(lights->getPixels(), 255));
  lights->getPixels()->setBrightness(40);
  lights->setState(SLStateWakingUp);
}

//  0s: wake up
//  5s: listen
// 10s: yes or error
// 15s: shut down
// 20s: restart

SLTimeInterval startTime = millis();
bool showError = false;

void loop() {
  SLState state = lights->getState();
  SLTimeInterval offset = millis() - startTime;
  if (state != SLStateListening && 5000 <= offset && offset < 10000) {
    Serial.println("Set listening");
    lights->setState(SLStateListening);
  } else if (state != SLStateError && state != SLStateStandby && 10000 <= offset && offset < 15000) {
    if (showError) {
      Serial.println("Set error");
      lights->setState(SLStateError);
    } else {
      Serial.println("Set yes");
      lights->setState(SLStateStandby);
    }
  } else if (state != SLStateNone && 15000 <= offset) {
    Serial.println("Set none");
    lights->setState(SLStateNone);
  } else if (20000 <= offset) {
    Serial.println("\n---\n");
    lights->setState(SLStateWakingUp);
    startTime = millis();
    showError = !showError;
  }
  lights->step();
}
