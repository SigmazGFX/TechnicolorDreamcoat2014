#ifndef Snake_h
#define Snake_h

#include "FastLED.h"
#include "Effect.cpp"

#define SNAKE_LENGTH 16

class Snake : public Effect {
  
  private:
  
    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };
    
    struct Pixel {
        uint8_t x;
        uint8_t y;
    };
    
    CRGB colours[SNAKE_LENGTH];
    
    uint8_t initialHue;
    
    Pixel pixels[SNAKE_LENGTH];
    
    Direction direction;
    
    void newDirection() {
        switch (direction) {
            case UP:
            case DOWN:
                direction = random(0, 2) == 1 ? RIGHT : LEFT;
                break;
                
            case LEFT:
            case RIGHT:
                direction = random(0, 2) == 1 ? DOWN : UP;
                
            default:
                break;
        }
    }

    void shuffleDown() {
        for (byte i = SNAKE_LENGTH - 1; i > 0; i--) {
            pixels[i] = pixels[i - 1];
        }
    }


  public:
    Snake(CRGB *leds) : Effect(leds, "Snake"), initialHue(0) {}
    
    virtual void draw(EffectControls controls) {
        shuffleDown();
        if (random(10) > 6) {
            newDirection();
        }
        switch (direction) {
            case UP:
                pixels[0].y = (pixels[0].y + 1) % HEIGHT;
                break;
            case LEFT:
                pixels[0].x = (pixels[0].x + 1) % WIDTH;
                break;
            case DOWN:
                pixels[0].y = pixels[0].y == 0 ? HEIGHT - 1 : pixels[0].y - 1;
                break;
            case RIGHT:
                pixels[0].x = pixels[0].x == 0 ? WIDTH - 1 : pixels[0].x - 1;
                break;
        }
        fill_rainbow(colours, SNAKE_LENGTH, initialHue++);
        for (uint8_t i = 0; i < SNAKE_LENGTH; i++) {
            pixel(pixels[i].x, pixels[i].y) = colours[i] %= (255 - i * (255 / SNAKE_LENGTH));
        }
        blur2d(leds, WIDTH, HEIGHT, 32);
    }

};

#endif



