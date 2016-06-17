#ifndef Perlin_h
#define Perlin_h

#include "FastLED.h"
#include "Effect.cpp"

#define MAX_DIMENSION WIDTH

class Perlin : public Effect {
  
  private:
    uint8_t hue;
    uint16_t speed;
    uint16_t scale;
    uint16_t funkyScale;
    uint16_t x;
    uint16_t y;
    uint16_t z;
    
    uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
  
    CRGBPalette16 currentPalette;
    uint8_t colorLoop;  
    uint8_t paletteIndex;
    uint8_t paletteCount;
    
  public:
    Perlin(CRGB *leds) : Effect(leds, "Perlin"), 
            hue(0), 
            speed(2), 
            scale(30), 
            funkyScale(0), 
            currentPalette(PartyColors_p), 
            colorLoop(1), 
            paletteIndex(0), 
            paletteCount(10) {
        x = random16();
        y = random16();
        z = random16();
    }
    
    virtual void draw(EffectControls controls) {
        if (controls.optionButton) {
            ChangePalette();
        }
        if (controls.volume > controls.optionPot && funkyScale == 0) {
//            Serial.println("Beat detected... ");
            funkyScale = 16;
        }
        if (funkyScale > 0) {
            funkyScale--;
        }
        
        fillnoise8();
        mapNoiseToLEDsUsingPalette();
    }
    
    void fillnoise8() {
        // If we're runing at a low "speed", some 8-bit artifacts become visible
        // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
        // The amount of data smoothing we're doing depends on "speed".
        uint8_t dataSmoothing = 0;
        if (speed + funkyScale < 50) {
            dataSmoothing = 200 - ((speed + funkyScale) * 4);
        }
      
        for (int i = 0; i < MAX_DIMENSION; i++) {
            int ioffset = scale * i;
            for (int j = 0; j < MAX_DIMENSION; j++) {
                int joffset = scale * j;
                
                byte data = inoise8(x + ioffset, y + joffset, z);
          
                // The range of the inoise8 function is roughly 16-240.
                // These two operations expand those values out to roughly 0..255
                // You can comment them out if you want the raw noise data.
                data = qsub8(data, 16);
                data = qadd8(data, scale8(data, 39));
          
                if (dataSmoothing) {
                    uint8_t olddata = noise[i][j];
                    uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                    data = newdata;
                }
                
                noise[i][j] = data;
            }
        }
        
        z += (speed + funkyScale);
        
        // apply slow drift to X and Y, just for visual variation.
        x += (speed + funkyScale) / 8;
        y -= (speed + funkyScale) / 16;
    }
    
    void mapNoiseToLEDsUsingPalette() {
        static uint8_t ihue = 0;
        
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
              // We use the value at the (i,j) coordinate in the noise
              // array for our brightness, and the flipped value from (j,i)
              // for our pixel's index into the color palette.
        
              uint8_t index = noise[j][i];
              uint8_t bri =   noise[i][j];
        
              // if this palette is a 'loop', add a slowly-changing base value
              if (colorLoop) { 
                  index += ihue;
              }
        
              // brighten up, as the color palette itself often contains the 
              // light/dark dynamic range desired
              if (bri > 127) {
                  bri = 255;
              } else {
                  bri = dim8_raw(bri * 2);
              }
        
              pixel(i, j) = ColorFromPalette(currentPalette, index, bri);
            }
        }
        ihue += 1;
    }
    
    void ChangePalette() {
        switch (paletteIndex++ % paletteCount) {
            case 0: 
              currentPalette = RainbowColors_p; speed = 2; scale = 30; colorLoop = 1;
              break;
              
            case 1:
              SetupPurpleAndGreenPalette(); speed = 10; scale = 50; colorLoop = 1;
              break;
              
            case 2:
              SetupBlackAndWhiteStripedPalette(); speed = 4; scale = 30; colorLoop = 1;
              break;
              
            case 3:
              currentPalette = CloudColors_p; speed =  4; scale = 30; colorLoop = 0;
              break;
              
            case 4:
              currentPalette = LavaColors_p; speed =  8; scale = 50; colorLoop = 0;
              break;
              
            case 5:
              currentPalette = PartyColors_p; speed = 20; scale = 30; colorLoop = 1;
              break;
              
            case 6:
              SetupRandomPalette(); speed = 20; scale = 20; colorLoop = 1;
              break;
              
            case 7:
              SetupRandomPalette(); speed = 50; scale = 50; colorLoop = 1;
              break;
              
            case 8:
              SetupRandomPalette(); speed = 90; scale = 90; colorLoop = 1;
              break;
              
            case 9:
              currentPalette = RainbowStripeColors_p; speed = 30; scale = 20; colorLoop = 1;
              break;
        }
    }

    // This function generates a random palette that's a gradient
    // between four different colors.  The first is a dim hue, the second is 
    // a bright hue, the third is a bright pastel, and the last is 
    // another bright hue.  This gives some visual bright/dark variation
    // which is more interesting than just a gradient of different hues.
    void SetupRandomPalette()
    {
      currentPalette = CRGBPalette16( 
                          CHSV( random8(), 255, 32), 
                          CHSV( random8(), 255, 255), 
                          CHSV( random8(), 128, 255), 
                          CHSV( random8(), 255, 255)); 
    }
    
    // This function sets up a palette of black and white stripes,
    // using code.  Since the palette is effectively an array of
    // sixteen CRGB colors, the various fill_* functions can be used
    // to set them up.
    void SetupBlackAndWhiteStripedPalette()
    {
      // 'black out' all 16 palette entries...
      fill_solid(currentPalette, 16, CRGB::Black);
      // and set every fourth one to white.
      currentPalette[0] = CRGB::White;
      currentPalette[4] = CRGB::White;
      currentPalette[8] = CRGB::White;
      currentPalette[12] = CRGB::White;
    }
    
    // This function sets up a palette of purple and green stripes.
    void SetupPurpleAndGreenPalette()
    {
      CRGB purple = CHSV( HUE_PURPLE, 255, 255);
      CRGB green  = CHSV( HUE_GREEN, 255, 255);
      CRGB black  = CRGB::Black;
      
      currentPalette = CRGBPalette16( 
        green,  green,  black,  black,
        purple, purple, black,  black,
        green,  green,  black,  black,
        purple, purple, black,  black );
    }

};

#endif




