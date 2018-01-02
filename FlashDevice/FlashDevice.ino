#include <elapsedMillis.h>
 //DEFINING PINS || DO NOT CHANGE
const int LEDS = 0;
const int BUTTONLED = 1;
const int FET = 2;
const int BUTTONVALUE = 4;

//TIMERS
long flashFrequency = 1000/30;     // interval at which to blink (milliseconds)
long flashTime = 3; // Total time the flash flashes in seconds
float delayTime = 1;
float throwDelay = 2; //Delay after releasing button in seconds. 
float SRD = 0.33; // Delay between blinks during find mode in seconds

//INTERNAL STATES || DO NOT TOUCH
boolean button = true; // INTERNAL BOOLEAN FOR TELLING THE STATE OF BUTTON
boolean start = true; // WHETHER START LOOP IS ACTIVE OR NOT
boolean flashActivated = false;  // STATE OF FLASH: FLASHING OR NOT FLASHING
int sensorValue = 0; // BUTTON STATE INPUT
int counter = 0; // COUNTER FOR COUNTING TIME BUTTON IS PRESSED
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0; // will store last time LED was updated
long previousMillisFindMode = 0;    // will store last time LED was updated
long previousMillisElapsedTime = 0; // will store last time LED was updated
elapsedMillis timeElapsed;
unsigned long flashElapsed;
boolean initialFlash = true;
boolean findModeActive = false; 

void setup() {
  start = true;
  // initialize the digital pin as an output.
  pinMode(LEDS, OUTPUT); //LEDs
  pinMode(FET, OUTPUT); // Power FET
  pinMode(BUTTONLED, OUTPUT); // Puwer Button LED
  pinMode(BUTTONVALUE, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  if (start == true) {
    firstBoot();
  }
  if (flashActivated == true && button == false) {
    if(initialFlash == true){
      flashElapsed = timeElapsed;
    }
    initialFlash = false;
    flash();
  }
  if (findModeActive == true){
    findMode();
    }
  checkButton();
  checkCounter();
}

void firstBoot() {
  digitalWrite(BUTTONLED, LOW);
  for (int i = 0; i < 7; i++) {
    digitalWrite(BUTTONLED, HIGH);
    delay(140 - (i * 20));
    digitalWrite(BUTTONLED, LOW);
    delay(140 - (i * 20));
  }
  digitalWrite(FET, HIGH);
  digitalWrite(BUTTONLED, HIGH);

}

void flash() {
  
  unsigned long currentMillisElapsedTime = millis();
  unsigned long currentMillis = millis();

  if (timeElapsed - flashElapsed >= throwDelay * 1000 ){
    if (currentMillis - previousMillis >= flashFrequency)
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW){
        ledState = HIGH;
      }else{
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(LEDS, ledState);
    }
  }

  if(millis() - flashElapsed >= flashTime * 1000 + throwDelay * 1000){
    ledState = LOW;
    digitalWrite(LEDS, LOW);
    previousMillisElapsedTime = currentMillisElapsedTime;
    flashActivated = false;
    findModeActive = true;
  }
  // for (int i = 0; i < 50; i++) {
  //   digitalWrite(LEDS, HIGH);
  //   delay(1000 / 30);
  //   digitalWrite(LEDS, LOW);
  //   delay(1000 / 30);
  // }
  // findMode();
}

//Variable timer function -> Set boolean to false after timer runs out. Non blocking code. (Button to cancel)
void findMode() {
  if (millis() - previousMillisFindMode >= 500){
    previousMillisFindMode = millis();
    if (ledState == LOW){
      ledState = HIGH;
    }
    else{
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(BUTTONLED, ledState);
  }
}

void checkButton() {
  sensorValue = digitalRead(BUTTONVALUE);
  if(sensorValue == HIGH){
    if (start == true) {
      start = false;
    }
  }else{
  }

  //  if (currentMillis - previousMillis > interval) {
  //    // save the last time you blinked the LED
  //    previousMillis = currentMillis;
  if (sensorValue == HIGH ) {
    if (flashActivated != true) {
      digitalWrite(BUTTONLED, HIGH);
      delay(50);
      digitalWrite(BUTTONLED, LOW);
      delay(50);
      button = true;
      checkCounter();
      counter++;
    }
    if(findModeActive == true){
      digitalWrite(BUTTONLED, HIGH);
      button = true;
      findModeActive = false;
      digitalWrite(FET, LOW);
      delay(2000);
    }

  } else if (sensorValue == LOW && findModeActive == false) {
    digitalWrite(BUTTONLED, LOW);
    button = false;
    counter = 0;
  } else {
    button = false;
    counter = 0;
  }
  checkCounter();
}

void checkCounter() {
  if (counter >= delayTime) {
    digitalWrite(BUTTONLED, HIGH);
    flashActivated = true;
  }
}
