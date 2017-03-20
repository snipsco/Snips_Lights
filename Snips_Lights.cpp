#include "Adafruit_NeoPixel.h"
#include "Snips_Lights.h"

Snips_Lights::Snips_Lights(uint16_t n, uint8_t p, neoPixelType t) :
  _pixels(n, p, t), _previousRotationIndex(0)
{
  _pixels.begin();
  setState(SLStateNone);
  setPrimaryColor(_pixels.Color(255, 255, 255));
  setSecondaryColor(_pixels.Color(0, 0, 255));
  setErrorColor(_pixels.Color(255, 0, 0));
}

void Snips_Lights::setPrimaryColor(SLColor color) {
  primaryColor = color;
}

void Snips_Lights::setSecondaryColor(SLColor color) {
  secondaryColor = color;
}

void Snips_Lights::setErrorColor(SLColor color) {
  errorColor = color;
}

void Snips_Lights::setState(SLState newState) {
  _currentState = newState;
  _currentFrame = 0;
  switch (_currentState) {
    case SLStateNone:
      _animationParameters = { 0, 0 };
      setAllPixels(SLBLACK);
      break;
    case SLStateWakingUp:
      _animationParameters = { 30, pixelCount() };
      break;
    case SLStateStandby:
      _animationParameters = { 300, 0 };
      setAllPixels(primaryColor);
      break;
    case SLStateListening:
      _animationParameters = { 100, 0 };
      setAllPixels(secondaryColor);
      break;
    case SLStateLoading:
      _animationParameters = { 100, 0 };
      break;
    case SLStateYes:
      _animationParameters = { 100, pixelCount() };
      break;
    case SLStateError:
      _animationParameters = { 1000, 1 };
      setAllPixels(errorColor);
      break;
    case SLStateShuttingDown:
      _animationParameters = { 30, pixelCount() };
      break;
    default:
      // Should never happen
      while (true) {}
  }
}

void Snips_Lights::transitionToNextState() {
  switch (_currentState) {
    case SLStateWakingUp:
    case SLStateYes:
    case SLStateError:
      setState(SLStateStandby);
      break;
    case SLStateShuttingDown:
      setState(SLStateNone);
      break;
    case SLStateNone:
    case SLStateStandby: 
    case SLStateListening:
    case SLStateLoading:
    default:
      // Should never happen
      while (true) {}
  }
}

void Snips_Lights::step() {
  if (0 == _animationParameters.period) return;
  if (0 != _animationParameters.maxFrame && 
    _animationParameters.maxFrame <= _currentFrame) { 
    transitionToNextState(); 
    return;
  }

  SLPixelIndex count = pixelCount();
  SLPixelIndex prev = _previousRotationIndex++;
  _previousRotationIndex %= count;
  SLPixelIndex curr = _previousRotationIndex;
  switch (_currentState) {
    case SLStateNone:
      return;
    case SLStateWakingUp:
      _pixels.setPixelColor(curr, primaryColor);
      _pixels.show();
      break;
    case SLStateStandby:
      setPixel(curr, SLBLACK);
      setPixel(prev, primaryColor);
      _pixels.show();
      Serial.println("SLStateStandby");
      break;
    case SLStateListening:
      setPixel(curr, SLBLACK);
      setPixel(prev, secondaryColor);
      _pixels.show();
      Serial.println("SLStateListening");
      break;
    case SLStateLoading:
      setPixel(curr, primaryColor);
      setPixel(prev, secondaryColor);
      _pixels.show();
      Serial.println("SLStateLoading");
      break;
    case SLStateYes:
      setPixel(curr, primaryColor);
      _pixels.show();
      Serial.println("SLStateYes");
      break;
    case SLStateError:
      Serial.println("SLStateError");
      break;
    case SLStateShuttingDown:
      setPixel(curr, SLBLACK);
      _pixels.show();
      Serial.println("SLStateShuttingDown");
      break;
    default:
      // Should never happen
      while (true) {}
  }
  delay(_animationParameters.period);
  _currentFrame = _currentFrame + 1;
}

void Snips_Lights::setPixel(SLPixelIndex index, SLColor color) {
  _pixels.setPixelColor(index % pixelCount(), color);
}

void Snips_Lights::setAllPixels(SLColor color) {
  for (SLPixelIndex i = 0; i < _pixels.numPixels(); i++) {
    _pixels.setPixelColor(i, color);
  }
  _pixels.show();
}
