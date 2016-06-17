#ifndef FireEffect_h
#define FireEffect_h

#include "FastLED.h"
#include "Effect.cpp"

#define COOLING 70
#define SPARKING 120

class FireEffect : public Effect {
  
  private:
    CRGBPalette16 palette;
    uint8_t heats[WIDTH][HEIGHT];
    uint8_t frame;
  
  public:
    FireEffect(CRGB *leds, CRGBPalette16 palette) : Effect(leds, "Fire 2012"), palette(palette), frame(0) {
    }
    
    virtual void draw(EffectControls controls) {
        for (int columnIdx = 0; columnIdx < WIDTH; columnIdx++) {
        
            random16_add_entropy(random());
            
            // Step 1.  Cool down every cell a little  
            for (int i = 0; i < HEIGHT; i++) {
                heats[columnIdx][i] = qsub8(heats[columnIdx][i], random8(0, ((COOLING * 10) / HEIGHT) + 2));
            }
        
            // Step 2.  Heat from each cell drifts 'up' and diffuses a little
            for (int k = HEIGHT - 1; k > 1; k--) {
                heats[columnIdx][k] = (heats[columnIdx][k - 1] + heats[columnIdx][k - 2] + heats[columnIdx][k - 2] ) / 3;
            }
            
            // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
            if (controls.optionPot > random8()) {
                int y = random8(5);
                heats[columnIdx][y] = qadd8(heats[columnIdx][y], random8(160, 255));
            }
            
            // Step 4.  Map from heat cells to LED colors
            for (int j = 0; j < HEIGHT; j++) {
                // Scale the heat value from 0-255 down to 0-240
                // for best results with color palettes.
                byte colorindex = scale8(heats[columnIdx][j], 240);
                pixel(columnIdx, j) = ColorFromPalette(palette, colorindex);
            }
        }
    }
};

#endif



