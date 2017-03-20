# Snips_Lights
Arduino library for light animations for the smart speaker project.

Depends on the Adafruit_NeoPixel library by Adafruit. This is the official library for the NeoPixel ring, but may not be the best choice in the long term.

This Library contains one C++ class, `Snips_Lights` the main features of this class are:

1. the initialization, that happens by passing a reference to an instance of `Adafruit_Neopixel`
2. the setting of the state, represented by the `SLState` enum
3. stepping forward one frame, by calling the `step()` method, that also includes a call to `delay()`

If the state has a finite animation, it automatically begins the next state after that. So the animation at wake up transitions to the standby animation after the last frame. The standby animation could last forever without some sort of outside communication. This outside communication is not managed by `Snips_Lights`.

You can find an arduino sketch that shows that shows all the animations in the examples folder.
