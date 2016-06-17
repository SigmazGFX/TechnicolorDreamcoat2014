#ifndef MiniClouds_h
#define MiniClouds_h

#include "FastLED.h"
#include "Effect.cpp"
#include "FunkyClouds.cpp"

class MiniClouds : public FunkyClouds {
  
  private:
      timer multiTimer[7];
  
  public:
        MiniClouds(CRGB *leds) : FunkyClouds(leds, "Mini Clouds", multiTimer, 7) {
        multiTimer[0].tact = 55;     //x1
        multiTimer[0].up = WIDTH - 1;
        multiTimer[0].down = 0;
    
        multiTimer[1].tact = 50;     //y1
        multiTimer[1].up = HEIGHT - 1;
        multiTimer[1].down = 0;
    
        multiTimer[2].tact = 3;      //color
        multiTimer[2].up = 255;
        multiTimer[2].down = 0;
    
        multiTimer[3].tact = 76;     //x2  
        multiTimer[3].up = WIDTH - 1;
        multiTimer[3].down = 0;
    
        multiTimer[4].tact = 125;     //y2
        multiTimer[4].up = HEIGHT - 1;
        multiTimer[4].down = 0;
    
        multiTimer[5].tact = 73;    //center spiral x
        multiTimer[5].up = HEIGHT - 4;
        multiTimer[5].down = 0;
    
        multiTimer[6].tact = 145;    //center spiral y
        multiTimer[6].up = HEIGHT - 4;
        multiTimer[6].down = 0;
        
        for (int i = 0; i < timers; i++) {
            multiTimer[i].delta = 1;
            multiTimer[i].count = random(multiTimer[i].up);
        }
    }
    
    void draw(EffectControls controls) {
        UpdateTimers();
    
        // the "seed": 3 moving dots
        pixel(multiTimer[0].count, multiTimer[1].count) = CHSV(multiTimer[2].count, 255, 255);
        pixel(multiTimer[3].count, multiTimer[4].count) = CHSV(255 - multiTimer[2].count, 255, 255);
    
        // coordinates are the average of 2 oscillators
        pixel((multiTimer[0].count + multiTimer[1].count) / 2, (multiTimer[3].count + multiTimer[4].count) / 2) = CHSV(multiTimer[2].count / 2, 255, 255);
    
        // the balance of the (last) values of the following functions affects the
        // appearence of the effect a lot
    
        // a moving spiral
        SpiralStream(multiTimer[5].count, multiTimer[6].count, 3, 210); // play here
    
        // y wind
        StreamVertical(140);    // and here
    
        // x wind
        StreamHorizontal(160);  // and here
    
        // main spiral
        SpiralStream(15, 15, 15, 150); // and here      
    }

};

#endif



