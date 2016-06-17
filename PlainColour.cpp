#ifndef PlainColour_h
#define PlainColour_h

#include "FastLED.h"
#include "Effect.cpp"

class PlainColour : public Effect {
  
  private:
    CRGB colour;
  
  public:
    PlainColour(CRGB *leds, char const *name, CRGB colour) : Effect(leds, name), colour(colour) {}
    
    virtual void draw(EffectControls controls) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = colour;
      }
    }

};

#endif




