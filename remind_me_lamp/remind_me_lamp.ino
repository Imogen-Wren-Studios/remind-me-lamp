/* Remind Me Light!

 Dont forget that you did the thing.
 
 My version will have 4 LEDs and 3 Buttons.
 
Light lights up untill task is done

- Yello: Take Pills
- Blue: Take Morning Gel
- Pink: Take Evening Gel
- Green: All Tasks Complete
 

*/



// Or could use fastLED

#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 5
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

#define RANDOMSEED_PIN A2

#define LIGHT_SENSE_PIN A0
#define LIGHT_SENSE_DAY_LEVEL 700
#define LIGHT_SENSE_NIGHT_LEVEL 800

bool night_mode = false;
bool last_mode;

#define BLUE_BUTTON_PIN 13
#define BUTTON_ZERO_PIN A3
#define BUTTON_ONE_PIN A6
#define BUTTON_TWO_PIN A7
#define BUTTON_THREE_PIN 3

#define LED_DATA_PIN 5

#include <buttonObject.h>

#include <autoDelay.h>

autoDelay sampleDelay;

#define SAMPLE_DELAY_S 1

buttonObject buttonArray[5] = {
  buttonObject(BUTTON_ZERO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_ONE_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_TWO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_THREE_PIN, BUTTON_PULL_LOW),
  buttonObject(BLUE_BUTTON_PIN, BUTTON_PULL_LOW)
};  // Set up instance of buttonObject. Pass Button Pin & whether it pulls HIGH, or LOW when pressed.



void sampleLDR() {
  if (sampleDelay.secondsDelay(SAMPLE_DELAY_S)) {
    int lightLevel = analogRead(LIGHT_SENSE_PIN);
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.print("   Night Mode: ");
    if (lightLevel >= LIGHT_SENSE_NIGHT_LEVEL) {
      Serial.println("Nighttime");
      night_mode = true;
      leds[0] = CHSV(0, 0, 0);
    } else {
      Serial.println("Daytime");
      night_mode = false;
      if (night_mode != last_mode){
        Serial.println("New Day! ");
        allOn();
      }
    }
    last_mode = night_mode;
  }
}

void allOn() {
  uint8_t startHue = random8();
  Serial.print("Start Hue: ");
  Serial.println(startHue);
  // leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV(startHue + (i * 10), 255, BRIGHTNESS);
  }
}


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(RANDOMSEED_PIN));
  pinMode(BUTTON_ZERO_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);
  pinMode(BUTTON_THREE_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  for (int i = 0; i < 5; i++) {
    buttonArray[i].begin();
  }
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  allOn();
  leds[0] = CHSV(0, 0, 0);
}


void loop() {


  sampleLDR();

  for (int i = 0; i < 5; i++) {
    buttonArray[i].buttonLoop();
  }

  if (buttonArray[0].shortPress) {
    Serial.println("Button Zero Pressed");
    buttonArray[0].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
    leds[1] = CHSV(0, 0, 0);
  }

  if (buttonArray[1].shortPress) {
    Serial.println("Button One Pressed");
    buttonArray[1].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
    leds[2] = CHSV(0, 0, 0);
  }

  if (buttonArray[2].shortPress) {
    Serial.println("Button Two Pressed");
    buttonArray[2].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
    leds[3] = CHSV(0, 0, 0);
  }

  if (buttonArray[3].shortPress) {
    Serial.println("Button Three Pressed");
    buttonArray[3].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
    leds[4] = CHSV(0, 0, 0);
  }

  if (buttonArray[4].shortPress) {
    Serial.println("Blue Button Pressed");
    buttonArray[4].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
    allOn();
   // leds[0] = CHSV(0, 0, 0);
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
