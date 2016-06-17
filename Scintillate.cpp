#ifndef Scintillate_h
#define Scintillate_h

#include "FastLED.h"
#include "Effect.cpp"

#define CONCURRENT_SPARKLES (NUM_LEDS/20)

struct Sparkle {
    uint8_t x, y;
    uint8_t colourIndex;
};

static CRGB sparklePalette[] = {CRGB::Black, CRGB::White, CRGB::DarkRed};

class Scintillate : public Effect {
  
  private:
    bool soundReactive;
    uint8_t brightness;
    Sparkle sparkles[CONCURRENT_SPARKLES];
    
  public:
    Scintillate(CRGB *leds) : Effect(leds, "Scintillate"), soundReactive(false), brightness(0) {
    }
    
    virtual void draw(EffectControls controls) {
        for (int i = 0; i < CONCURRENT_SPARKLES; i++) {
            if (sparkles[i].colourIndex == 0) {
                if (random() > 128) {                  
                    sparkles[i].x = random(WIDTH);
                    sparkles[i].y = random(HEIGHT);
                    sparkles[i].colourIndex = 1;
                }                
            } else if (sparkles[i].colourIndex == 2) {
                sparkles[i].colourIndex = 0;                
            } else {
                sparkles[i].colourIndex++;
            }
            pixel(sparkles[i].x, sparkles[i].y) = CRGB(sparklePalette[sparkles[i].colourIndex]);
        }
    }
    
};

#endif




