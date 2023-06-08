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
#define LIGHT_SENSE_DAY_LEVEL 1000
#define LIGHT_SENSE_NIGHT_LEVEL 800



#define BLUE_BUTTON_PIN 13
#define BUTTON_ZERO_PIN A3
#define BUTTON_ONE_PIN A6
#define BUTTON_TWO_PIN A7
#define BUTTON_THREE_PIN 3

#define LED_DATA_PIN 5

#include <buttonObject.h>

#include <autoDelay.h>

autoDelay sampleDelay;
autoDelay nightDelay;

#define SAMPLE_DELAY_S 1
#define NIGHT_MINUTES_DELAY 240  // 60*4 =

buttonObject buttonArray[5] = {
  buttonObject(BUTTON_ZERO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_ONE_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_TWO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_THREE_PIN, BUTTON_PULL_LOW),
  buttonObject(BLUE_BUTTON_PIN, BUTTON_PULL_LOW)
};  // Set up instance of buttonObject. Pass Button Pin & whether it pulls HIGH, or LOW when pressed.

bool night_mode = false;
bool last_mode;

void sampleLDR() {
  if (sampleDelay.secondsDelay(SAMPLE_DELAY_S)) {
    int lightLevel = analogRead(LIGHT_SENSE_PIN);
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    if (lightLevel < LIGHT_SENSE_DAY_LEVEL) {
      Serial.println("Daylight Detected");
    }
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
  startDay();
  leds[0] = CHSV(0, 0, 0);  // turn off first LED
}


// state machine
/*

Init -> day mode.
- wait for user input
if all lights are off -> night mode.
night mode waits for at least 4 hours
- check LDR untill brightness > 1000
-> day mode
button [4] resets to daymode at any point
*/



bool initDayMode = false;
bool initNightMode = false;
bool startSensing = false;

void startDay() {
  for (int i; i < 5; i++) {
    ledStates[i] = true;
  }
  allOn();
}



void nightMode() {
  if nightDelay
    .minutesDelayEvent(NIGHT_MINUTES_DELAY) {
      startSensing = true;
    }
  if (startSensing) {
    sampleLDR();
  }
}

bool ledStates[5] = {
  1,
  1,
  1,
  1,
  1
}

void
applyLEDstates() {
  // Assume all lights always start from 1
  for (int i; i < 4; i++) {
    if (ledStates[i]) {
      // do nothing
    } else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
}

void loop() {

  // sampleLDR();  // This needs to change

  if (initDayMode) {
    Serial.println("Good Morning - New Day Initialised");
    startDay();
    initDayMode = false;
  }

  for (int i = 0; i < 5; i++) {
    buttonArray[i].buttonLoop();
    if (i < 4) {
      if (buttonArray[i].shortPress) {
        buffer[16];
        sprintf(buffer, "Button [ %i ] Pressed");
        Serial.println(buffer);
        buttonArray[i].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
        ledStates[i + 1] = false;
      }
    } else {
      if (buttonArray[4].shortPress) {
        Serial.println("Blue Button Pressed");
        buttonArray[4].buttonReset();  // .buttonReset method resets longPress & shortPress variables once action has been taken
        initDayMode = true;
        // leds[0] = CHSV(0, 0, 0);
      }
    }
  }

  if (initNightMode) {
  }

  applyLEDstates();

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
