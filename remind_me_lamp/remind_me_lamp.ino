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

#define LIGHT_SENSE_PIN A0
#define LIGHT_SENSE_DAY_LEVEL 200
#define LIGHT_SENSE_NIGHT_LEVEL 50

bool night_mode = false;

#define BLUE_BUTTON_PIN 13
#define BUTTON_ZERO_PIN A3
#define BUTTON_ONE_PIN A6
#define BUTTON_TWO_PIN A7
#define BUTTON_THREE_PIN 3

#define LED_DATA_PIN 5

#include <buttonObject.h>

buttonObject buttonArray[5] = {
  buttonObject(BLUE_BUTTON_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_ZERO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_ONE_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_TWO_PIN, BUTTON_PULL_LOW),
  buttonObject(BUTTON_THREE_PIN, BUTTON_PULL_LOW)
   };  // Set up instance of buttonObject. Pass Button Pin & whether it pulls HIGH, or LOW when pressed.

void setup() {
  Serial.begin(115200);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ZERO_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);
  pinMode(BUTTON_THREE_PIN, INPUT_PULLUP);

  for (int i = 0; i < 5; i++) {
    buttonArray[i].begin();
  }
}


void loop() {

  if (buttonArray[0].shortPress) {
    Serial.println("Blue Button Pressed");
  }
}