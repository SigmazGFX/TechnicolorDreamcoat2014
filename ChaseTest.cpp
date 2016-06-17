#ifndef ChaseTest_h
#define ChaseTest_h

#include "FastLED.h"
#include "Effect.cpp"

class ChaseTest : public Effect {
  
  private:
    int frame;
  
  public:
    ChaseTest(CRGB *leds) : Effect(leds, "Chase Test"), frame(0) {}
    
    void draw(EffectControls controls) {
      leds[frame++ % NUM_LEDS] = CRGB::White;
    }

};

#endif




