#ifndef SNIPS_LIGHTS_H
#define SNIPS_LIGHTS_H

// A combined color as defined in the Adafruit library
#define SLColor uint32_t
#define SLBLACK 0

// The index type for pixel iteration
#define SLPixelIndex uint16_t

// The index type for frame iteration
#define SLFrameIndex uint16_t

// Time interval in milliseconds
#define SLTimeInterval unsigned long

enum SLState {
  SLStateNone, SLStateWakingUp, SLStateStandby, SLStateListening,
  SLStateLoading, SLStateYes, SLStateError, SLStateShuttingDown
};

struct SLAnimationParameters {
  SLTimeInterval period;
  SLFrameIndex maxFrame;
};

class Snips_Lights {

 public:
  // It is your responsibility to deallocate pixels
  // after deallocating the returned instance
  Snips_Lights(Adafruit_NeoPixel *pixels);

  void
    setPrimaryColor(SLColor color),
    setSecondaryColor(SLColor color),
    setErrorColor(SLColor color),
    setState(SLState state),
    step();

 private:
  void 
    transitionToNextState(),
    setPixel(SLPixelIndex index, SLColor color),
    setAllPixels(SLColor color);
  Adafruit_NeoPixel
    *pixels;
  SLColor
    primaryColor,
    secondaryColor,
    errorColor;
  SLState
    _currentState;
  SLFrameIndex
    // used for continuing from previous rotation animation
    _previousRotationIndex,
    _currentFrame;
  SLPixelIndex
    pixelCount();
  SLAnimationParameters
    _animationParameters;
};

#endif // SNIPS_LIGHTS_H
