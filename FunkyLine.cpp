#ifndef FunkyLine_h
#define FunkyLine_h

#include "FastLED.h"
#include "Effect.cpp"
#include "FunkyClouds.cpp"

class FunkyLine : public FunkyClouds {
  
  private:
    timer multiTimer[5];
  
  public:
    FunkyLine(CRGB *leds) : FunkyClouds(leds, "Funky Line", multiTimer, 5) {
        multiTimer[0].tact = 32;     //x1
        multiTimer[0].up = WIDTH - 1;
        multiTimer[0].down = 0;
        multiTimer[0].count = 0;
    
        multiTimer[1].tact = 35;     //y1
        multiTimer[1].up = HEIGHT - 1;
        multiTimer[1].down = 0;
        multiTimer[1].count = 0;        
    
        multiTimer[2].tact = 3;      //color
        multiTimer[2].up = 255;
        multiTimer[2].down = 0;
        multiTimer[2].count = 0;
        
        multiTimer[3].tact = 41;     //x2  
        multiTimer[3].up = WIDTH - 1;
        multiTimer[3].down = 0;
        multiTimer[3].count = 0;        
    
        multiTimer[4].tact = 49;     //y2
        multiTimer[4].up = HEIGHT - 1;
        multiTimer[4].down = 0;
        multiTimer[4].count = 0;        
    
        for (int i = 0; i < timers; i++) {
            multiTimer[i].delta = 1;
        }      
    }
    
    void draw(EffectControls controls) {
//        Serial.println("Updating timers...");
        // let the oscillators swing
        UpdateTimers();

        Line(multiTimer[3].count,
            multiTimer[4].count,
            multiTimer[0].count,
            multiTimer[1].count,
            multiTimer[2].count);        
            
          // y wind
        StreamVertical(120);    // and here

        // x wind
        StreamHorizontal(110);  // and here

    }

};

#endif



