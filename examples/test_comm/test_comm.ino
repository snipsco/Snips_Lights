#include "Adafruit_NeoPixel.h"
#include "Snips_Lights.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    int next = Serial.read();
    SLState newState = SLStateNone;
    UPDATE_STATE(&newState, next);
    PRINT_STATE(newState);
  }
}
