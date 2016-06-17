#ifndef FunkyClouds_h
#define FunkyClouds_h

#include "FastLED.h"
#include "Effect.cpp"

struct timer {
    unsigned long tact;
    unsigned long lastMillis;
    unsigned long count;
    int delta;
    byte up;
    byte down;
};

class FunkyClouds : public Effect {
  
      private:
      
      public:      
          int timers;
          timer *multiTimer;
    
          FunkyClouds(CRGB *leds, char const *name, timer multiTimer[], int timers) : Effect(leds, name), timers(timers), multiTimer(multiTimer) {}
      
          // counts everything with different speeds linear up and down
          // = oscillators following a triangular function
          void UpdateTimers()
          {
//              Serial.print("FunkyClouds::UpdateTimers(), timers = "); Serial.println(timers);
              unsigned long now = millis();
              for (int i = 0; i < timers; i++)
              {
//                  Serial.print("Updating timers i = "); Serial.println(i);
                  while (now - multiTimer[i].lastMillis >= multiTimer[i].tact)
                  {
                      multiTimer[i].lastMillis += multiTimer[i].tact;
                      multiTimer[i].count = multiTimer[i].count + multiTimer[i].delta;
                      if ((multiTimer[i].count == multiTimer[i].up) || (multiTimer[i].count == multiTimer[i].down))
                      {
                          multiTimer[i].delta = -multiTimer[i].delta;
                      }
                  }
              }
          }
    
          // fade the image buffer arround
          // x, y: center   r: radius   dimm: fade down
          void SpiralStream(int x, int y, int r, byte dimm) {
              for (int d = r; d >= 0; d--) {                // from the outside to the inside
                  for (int i = x - d; i <= x + d; i++) {
                      if (y - d < maxY(i + 1)) {
                          pixel(i, y - d) += pixel(i + 1, y - d);   // lowest row to the right
                          pixel(i, y - d).nscale8(dimm);
                      }
                  }
                  for (int i = y - d; i <= y + d; i++) {
                      if (i + 1 < maxY(x + d)) {
                          pixel(x + d, i) += pixel(x + d, i + 1);   // right colum up
                          pixel(x + d, i).nscale8(dimm);
                      }
                  }
                  for (int i = x + d; i >= x - d; i--) {
                      if (y + d < maxY(i - 1)) {
                          pixel(i, y + d) += pixel(i - 1, y + d);   // upper row to the left
                          pixel(i, y + d).nscale8(dimm);
                      }
                  }
                  for (int i = y + d; i >= y - d; i--) {
                      if (i - 1 < maxY(x -d)) {
                          pixel(x - d, i) += pixel(x - d, i - 1);   // left colum down
                          pixel(x - d, i).nscale8(dimm);
                      }
                  }
              }
          }
          
          // give it a linear tail
          void StreamVertical(byte scale)
          {
              for (int x = 0; x < WIDTH; x++) {
                  for (int y = 1; y < HEIGHT; y++) {
                      if (y - 1 < maxY(x)) {
                          pixel(x, y) += pixel(x, y - 1);
                          pixel(x, y).nscale8(scale);
                      }
                  }
              }
              for (int x = 0; x < WIDTH; x++) {
                  pixel(x, 0).nscale8(scale);
              }
          }
          
          // give it a linear tail
          void StreamHorizontal(byte scale)
          {
              for (int x = 1; x < WIDTH; x++) {
                  for (int y = 0; y < HEIGHT; y++) {
                      if (y < maxY(x - 1)) {
                          pixel(x, y) += pixel(x - 1, y);
                          pixel(x, y).nscale8(scale);
                      }
                  }
              }
              for (int y = 0; y < HEIGHT; y++) {
                  pixel(0, y).nscale8(scale);
              }
          }
          
          // Bresenham line
          void Line(int x0, int y0, int x1, int y1, byte color)
          {
              int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
              int dy = -abs(y1 - y0), sy = y0<y1 ? 1 : -1;
              int err = dx + dy, e2;
              for (;;) {
                  pixel(x0, y0) += CHSV(color, 255, 255);
                  if (x0 == x1 && y0 == y1) break;
                  e2 = 2 * err;
                  if (e2 > dy) { err += dy; x0 += sx; }
                  if (e2 < dx) { err += dx; y0 += sy; }
              }
          }
      
          
          // scale the brightness of the screenbuffer down
          void DimmAll(byte value)
          {
              for (int i = 0; i < NUM_LEDS; i++)
              {
                  leds[i].nscale8(value);
              }
          }
    
};

#endif



