#ifndef Starfield_h
#define Starfield_h

#include "FastLED.h"
#include "Effect.cpp"

#define MAX_STARS (WIDTH * HEIGHT / 15)

struct Star {
    uint16_t x;
    uint8_t y;
    uint8_t plane;
};

const uint8_t planes[] = {32, 92, 255};

class Starfield : public Effect {
  
  private:
    int frame;
    Star stars[MAX_STARS];
  
  public:
    Starfield(CRGB *leds) : Effect(leds, "Starfield"), frame(0) {
        for (int i = 0; i < MAX_STARS; i++) {
            stars[i].x = random16(WIDTH * 0xFF);
            stars[i].y = random8(HEIGHT);
            stars[i].plane = random8(3);
        }
    }
    
    void draw(EffectControls controls) {
        for (int i = 0; i < MAX_STARS; i++) {
            stars[i].x += (1 + stars[i].plane) * 13;
            
            // Check if our star is off the right of the screen
            if (stars[i].x > (WIDTH * 0xFF)) {
              
                // If so, put it back on the left
                stars[i].x = 0;

                // and randomly change the y position
//                stars[i].y = random8(HEIGHT);
                
                // and the plane, fuck it :-)
//                stars[i].plane = random8(3);
            }
            uint8_t actualPart = stars[i].x >> 8;
            uint8_t fractionalPart = stars[i].x & 0xFF;
            pixel(actualPart    , (uint8_t)stars[i].y) = CRGB(planes[stars[i].plane], planes[stars[i].plane], planes[stars[i].plane]) %= (0xFF - fractionalPart);
            pixel(actualPart + 1, (uint8_t)stars[i].y) = CRGB(planes[stars[i].plane], planes[stars[i].plane], planes[stars[i].plane]) %= fractionalPart;
        }
    }

};

#endif



