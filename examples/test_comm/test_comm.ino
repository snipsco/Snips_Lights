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
    printState(newState);
  }
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
