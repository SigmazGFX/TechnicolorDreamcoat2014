#ifndef AdvancingPaletteEffect_h
#define AdvancingPaletteEffect_h

#include "FastLED.h"
#include "Effect.cpp"

class AdvancingPaletteEffect : public Effect {
  
  private:
    uint8_t colourIndex;
    CRGBPalette16 palette;
  
  public:
    AdvancingPaletteEffect(CRGB *leds, char *name, CRGBPalette16 palette) : Effect(leds, name), 
      colourIndex(0),
      palette(palette) {}
    
    virtual void draw(EffectControls controls) {
      for (int i = 0; i < HEIGHT; i++) {     
        pixel(0, i) = ColorFromPalette(palette, colourIndex + i);
      }
      colourIndex++;
    }

};

#endif




