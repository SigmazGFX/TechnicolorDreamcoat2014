#ifndef PowerTest_h
#define PowerTest_h

#include "FastLED.h"
#include "Effect.cpp"

class PowerTest : public Effect {
  
  private:
    CRGB colour;
  
  public:
    PowerTest(CRGB *leds, CRGB colour) : Effect(leds, "Power Test"), colour(colour) {}
    
    virtual void draw(int rawPot, int rawMic, bool button) {
        int ledsIdx = 0;
        for (int i = 0; i < 100; i++) {
            leds[(ledsIdx += 37) % NUM_LEDS] = colour;
        }
    }

};

#endif




