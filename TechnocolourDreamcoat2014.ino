#include <FastLED.h>

#include "Effect.cpp"

//#include "LayoutTest.cpp"
#include "ChaseTest.cpp"
//#include "PowerTest.cpp"
//#include "PlainColour.cpp"
//#include "AdvancingPaletteEffect.cpp"
#include "FireEffect.cpp"
#include "Life.cpp"
#include "Plasma.cpp"
#include "Scintillate.cpp"
#include "Perlin.cpp"
#include "Snake.cpp"
//#include "SoundSaturation.cpp"
//#include "RotatingCube.cpp"
//#include "Mandelbrot.cpp"
//#include "RandomPainter.cpp"
//#include "FunkyLine.cpp"
//#include "MiniClouds.cpp"
#include "Starfield.cpp"

#define DATA_PIN 4
#define PROG_UP_PIN 2
#define PROG_DOWN_PIN 3
#define OPTION_BUTTON_PIN 7

#define BRIGHTNESS_PIN 21 // A7
#define VOLUME_PIN 20 // A6
#define OPTION_POT_PIN 19 // A5

#define FRAMES_PER_SECOND 60
#define DEBOUNCE_COUNT 16

DEFINE_GRADIENT_PALETTE(Pinkish_p) {
   0, 236,  63, 128,
  64,  96, 236,  36,
  96, 128, 236,  80,
 220,  64,  18, 255,
 255, 236,  63, 128    
};

CRGBPalette256 pinkish = Pinkish_p;

CRGB leds[NUM_LEDS];

//LayoutTest layoutTest(leds);
ChaseTest chaseTest(leds);

//PlainColour plainColourRed(leds, "Power Test (Red)", CRGB::Red);
//PlainColour plainColourGreen(leds, "Power Test (Green)", CRGB::Green);
//PlainColour plainColourBlue(leds, "Power Test (Blue)", CRGB::Blue);
//PlainColour plainColourWhite(leds, "Power Test (White)", CRGB::White);

//AdvancingPaletteEffect advancingPalette0(leds, HeatColors_p);
//AdvancingPaletteEffect advancingPalette1(leds, RainbowColors_p);
//AdvancingPaletteEffect advancingPalette2(leds, PartyColors_p);

FireEffect fire(leds, CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White));
Life life(leds);
Plasma plasma(leds, Pinkish_p);
Scintillate scintillate(leds);
Perlin perlin(leds);
Snake snake(leds);

//SoundSaturation soundSaturation(leds);
//
//RotatingCube cube(leds);
//Mandelbrot mandelbrot(leds);
//RandomPainter randomPainter(leds);
//
//FunkyLine funkyLine(leds);
//MiniClouds miniClouds(leds);
Starfield starfield(leds);

Effect* effects[] = {
  &chaseTest, &scintillate, &plasma, &perlin, &snake, &life, &fire, &starfield,
  NULL
};

EffectControls controls;

uint8_t effectIndex = 0;
uint8_t effectCount = 0;

void setup() {
//  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
//  FastLED.setCorrection(TypicalLEDStrip);

  set_max_power_indicator_LED(13);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 4000);

  pinMode(PROG_UP_PIN, INPUT_PULLUP);
  pinMode(PROG_DOWN_PIN, INPUT_PULLUP);
  pinMode(OPTION_BUTTON_PIN, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  while (effects[effectIndex++] != NULL) {
    effectCount++;
  }
  effectIndex = 0;

  delay(2000);
}

void loop() {
  readControls(&controls);
//    Serial.print("controls.progUp = "); Serial.print(controls.progUp);
//      Serial.print(", controls.progDown = "); Serial.print(controls.progDown);
//      Serial.print(", controls.optionButton = "); Serial.print(controls.optionButton);
//      Serial.print(", controls.brightness = "); Serial.print(controls.brightness);
//      Serial.print(", controls.optionPot = "); Serial.print(controls.optionPot);
//      Serial.print(", controls.volume = "); Serial.println(controls.volume);

  if (controls.progUp && controls.progDown) {
    // special mode?
  } else if (controls.progUp) {
    effectIndex = effectIndex == effectCount - 1 ? 0 : effectIndex + 1;
  } else if (controls.progDown) {
    effectIndex = effectIndex == 0 ? effectCount - 1 : effectIndex - 1;
  }

  Effect *effect = effects[effectIndex];
//    Serial.print("effectIndex: "); Serial.print(effectIndex);
//    Serial.print("effect.name(): "); Serial.println(effect->name());

  FastLED.setBrightness(controls.brightness);
  
  effect->draw(controls);

  FastLED.show();

  FastLED.clearData();
}

void readControls(EffectControls* controls) {
  if (controls->progUpDebounce == 0 && digitalRead(PROG_UP_PIN) == LOW) {
//    Serial.println("progUpDebounce");
    controls->progUpDebounce = DEBOUNCE_COUNT;
    controls->progUp = true;
  } else if (controls->progUpDebounce > 0) {
    controls->progUpDebounce--;
    controls->progUp = false;
  }

  if (controls->progDownDebounce == 0 && digitalRead(PROG_DOWN_PIN) == LOW) {
//    Serial.println("progDownDebounce");
    controls->progDownDebounce = DEBOUNCE_COUNT;
    controls->progDown = true;
  } else if (controls->progDownDebounce > 0) {
    controls->progDownDebounce--;
    controls->progDown = false;
  }

  if (controls->optionButtonDebounce == 0 && digitalRead(OPTION_BUTTON_PIN) == LOW) {
//    Serial.println("optionButtonDebounce");
    controls->optionButtonDebounce = DEBOUNCE_COUNT;
    controls->optionButton = true;
  } else if (controls->optionButtonDebounce > 0) {
    controls->optionButtonDebounce--;
    controls->optionButton = false;
  }

  controls->brightness =  map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 255);
  controls->volume = map(analogRead(VOLUME_PIN), 475, 1023, 0, 255);
  controls->optionPot = map(analogRead(OPTION_POT_PIN), 0, 1023, 0, 255);
}

constexpr int Effect::columnHeights[WIDTH];

uint16_t XY(uint8_t x, uint8_t y) {
  if (Effect::visible(x, y)) {
    uint16_t sum = 0;
    for (int i = 0; i < x; i++) {
      sum += Effect::columnHeights[i];
    }
    if (x & 0x01) {
      return sum + Effect::columnHeights[x] - y - 1;
    } else {
      return sum + y;
    }
  } else {
    return 0;
  }  
}

