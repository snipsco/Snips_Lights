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
      setAllPixels(SLBLACK);
      break;
    case SLStateStandby:
      _animationParameters = { 0, 0 };
      setAllPixels(primaryColor);
      break;
    case SLStateListening:
      _animationParameters = { 100, 0 };
      setAllPixels(secondaryColor);
      break;
    case SLStateError:
      _animationParameters = { 1000, 1 };
      setAllPixels(errorColor);
      break;
    default:
      // Should never happen
      while (true) {}
  }
  PRINT_STATE(newState);
}

void Snips_Lights::transitionToNextState() {
  switch (_currentState) {
    case SLStateWakingUp:
    case SLStateError:
      setState(SLStateStandby);
      break;
    case SLStateStandby:
    case SLStateListening:
    case SLStateNone:
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
  SLPixelIndex next = (curr + 1) % count;
  switch (_currentState) {
    case SLStateWakingUp:
      if (0 == _currentFrame % 2) {
        _pixels.setPixelColor(_currentFrame / 2, primaryColor);
      } else {
        _pixels.setPixelColor(pixelCount() - _currentFrame / 2 - 1, primaryColor);
      }
      _pixels.show();
      break;
    case SLStateListening:
      setPixel(curr, SLBLACK);
      setPixel(next, SLBLACK);
      setPixel(prev, secondaryColor);
      _pixels.show();
      break;
    case SLStateError:
      break;
    case SLStateNone:
    case SLStateStandby:
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
