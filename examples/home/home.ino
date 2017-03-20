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
}

SLTimeInterval startTime = millis();

void loop() {
  if (Serial.available()) {
    int next = Serial.read();
    SLState newState = SLStateNone;
    UPDATE_STATE(&newState, next);
    lights->setState(newState);
    printState(newState);
  }
  lights->step();
}

void printState(SLState state) {
  switch (state) {
    case SLStateNone:
      Serial.println("SLStateNone");
      break;
    case SLStateWakingUp:
      Serial.println("SLStateWakingUp");
      break;
    case SLStateStandby:
      Serial.println("SLStateStandby");
      break;
    case SLStateListening:
      Serial.println("SLStateListening");
      break;
    case SLStateLoading:
      Serial.println("SLStateLoading");
      break;
    case SLStateYes:
      Serial.println("SLStateYes");
      break;
    case SLStateError:
      Serial.println("SLStateError");
      break;
    case SLStateShuttingDown:
      Serial.println("SLStateShuttingDown");
      break;
    default:
      // Should never happen
      Serial.println("unkown");
      while (true) {}
  }
}
