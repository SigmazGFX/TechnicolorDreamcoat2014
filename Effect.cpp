#ifndef Effect_h
#define Effect_h

#include "FastLED.h"

#define WIDTH 36
#define HEIGHT 20
#define NUM_LEDS 640 // 38.4A, motherfuckers

//#define WIDTH 16
//#define HEIGHT 8
//#define NUM_LEDS 128 // 7.68A, motherfuckers

#define SOUND_THRESHOLD 128

struct EffectControls {
  
    uint8_t brightness;
    uint8_t volume;
    uint8_t optionPot;

    bool progUp;
    uint8_t progUpDebounce;
    
    bool progDown;
    uint8_t progDownDebounce;
    
    bool optionButton;
    uint8_t optionButtonDebounce;
    
    int rawMic;
    int rawPot;
};

class Effect {
  
  protected:
    CRGB *leds;
    char const *_name;
    CRGB deadPixel;
    uint8_t soundSaturationBrightness;
        
  public:
    static constexpr int columnHeights[WIDTH] = {
      20, 20, 20, 20, 18, 18, 18, 14, 12, 
      12, 14, 20, 20, 20, 20, 18, 18, 18, 
      18, 18, 18, 20, 20, 20, 20, 14, 12, 
      12, 14, 18, 18, 18, 20, 20, 20, 20
    };

    Effect(CRGB *leds, char const *name) : leds(leds), _name(name), deadPixel(CRGB::Black), soundSaturationBrightness(0) {}
    
    char const* name() {
        return _name;
    }

    virtual void draw(EffectControls controls) = 0;
    
    static bool visible(int16_t x, int16_t y) {
      return x >= 0 && y >= 0 && x < WIDTH && y < Effect::columnHeights[x];
    }
    
    static int16_t maxY(int16_t x) {
        return Effect::columnHeights[x];
    }
    
    struct CRGB& pixel(int16_t x, int16_t y) {
        if (visible(x, y)) {
            uint16_t sum = 0;
            for (int i = 0; i < x; i++) {
              sum += Effect::columnHeights[i];
            }
            if (x & 0x01) {
                return leds[sum + Effect::columnHeights[x] - y - 1];
            } else {
                return leds[sum + y];
            }
        } else {
            return deadPixel;
        } 
    }

//    bool visible(int16_t x, int16_t y) {
//      return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
//    }
//
//    int16_t maxY(int16_t x) {
//        return HEIGHT;
//    }
//
//    // Top Left origin
//    struct CRGB& pixel(int16_t x, int16_t y) {
//        if (y & 0x01) {
//            return leds[WIDTH * (HEIGHT - y - 1) + x];
//        } else {
//            return leds[WIDTH * (HEIGHT - y - 1) + WIDTH - x - 1];
//        }
//    }

// bottom left origin
//    struct CRGB& pixel(int16_t x, int16_t y) {
//        if (x & 0x01) {
//            return leds[HEIGHT * x + HEIGHT - y - 1];
//        } else {
//            return leds[HEIGHT * x + y];
//        }
//    }
    
    uint8_t normalisedPotVal(int rawPotVal) {
        uint8_t normalised = map(rawPotVal, 0, 1023, 0, 255);
        return normalised;
    }

    uint8_t normalisedMicVal(int rawMicVal, int rawPotVal) {
        uint8_t normalised = map((rawMicVal - 400) * rawPotVal, 1023 - 400, 1024 * 1024, 0, 255);
        return normalised;
    }

    void line(int8_t x1, int8_t y1, int8_t x2, int8_t y2) {
        //Serial.print("line("); Serial.print(x1); Serial.print(", ");Serial.print(y1); Serial.print(", "); Serial.print(x2); Serial.print(", ");Serial.print(y2); Serial.println(")");
        if ( (x1>=25 && x2>=25) || (y1>=25 && y2>=25) ) return;
        int8_t dx = abs(x2 -x1);
        int8_t dy = abs(y2 -y1);
      
        int8_t p1x,p1y,p2x,p2y,i;
  
        if (dx > dy) {
            if (x2>x1) {
                p1x=x1;
                p1y=y1;
                p2x=x2;
                p2y=y2;
            } else {
                p1x=x2;
                p1y=y2;
                p2x=x1;
                p2y=y1;
            }
          
            int8_t y = p1y;
            int8_t x = p1x;
            int8_t count = 0;
            int8_t increment = p2y > p1y ? 1 : -1;
            for (i=0; i<=dx; i++) {   
                count += dy;
                if (count > dx) {
                    count -= dx; 
                    y+= increment;
                }                         
                if (y>=0 && y<25 && x>=0 && x<25) {
                    pixel(x, y) = CRGB::White;
                }
                x++; 
                if (x>=25) {
                    break;
                }
            }
        } else {
            if (y2>y1) {
                p1x=x1;
                p1y=y1;
                p2x=x2;
                p2y=y2;
            } else {
                p1x=x2;
                p1y=y2;
                p2x=x1;
                p2y=y1;
            }
            int8_t y = p1y;
            int8_t x = p1x;
            int8_t count = 0;
            int8_t increment = p2x > p1x ? 1 : -1;
            for (i=0; i<=dy; i++) {   
                count += dx;
                if (count > dy) {
                    count -= dy; 
                    x+= increment;
                }                         
                if (y>=0 && y<25 && x>=0 && x<25) {
                    pixel(x, y) = CRGB::White;
                }
                y+=1; 
                if (y>=25) {
                    break;
                }
            }
        }    
    }
    
    void soundSaturate(EffectControls controls) {
        if (controls.volume > controls.optionPot) {
            //Serial.println("Saturating...");
            soundSaturationBrightness = 255;
        }
        if (soundSaturationBrightness > 0) {
            for (int i = 0; i < NUM_LEDS; i++) {
                if (leds[i]) {          
                    nblend(leds[i], CRGB::White, soundSaturationBrightness);
                }
            }
            soundSaturationBrightness = constrain(soundSaturationBrightness - 32, 0, 255);
        }
    }
    
};



#endif


