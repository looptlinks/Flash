// the setup routine runs once when you press reset:
int sensorValue = 0;
int oldSensorValue = 0;
float voltage = 0;
float oldVoltage = 0;
boolean button = true;
boolean start = true;
int p = 0;
int flashActivated = false;
int counter = 0;
int lowestV = 0;
int highestV = 0;
int delayTime = 1;
int throwDelay = 2; //Delay after releasing button in seconds. 
float SRD = 0.33; // Delay between blinks during find mode in seconds
long previousMillis = 0;        // will store last time LED was updated

long interval = 100;           // interval at which to blink (milliseconds)

byte flashingCounter = 0;

const int LEDS = 0;
const int FET = 2;
const int BUTTONLED = 1;
const int BUTTONVALUE = 4;

void setup() {
  start = true;
  // initialize the digital pin as an output.
  pinMode(LEDS, OUTPUT); //LEDs
  pinMode(FET, OUTPUT); // Power FET
  pinMode(BUTTONLED, OUTPUT); // Power Button LED
  pinMode(BUTTONVALUE, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  if (start == true) {
    firstBoot();
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
  delay(throwDelay * 1000);
  for (int i = 0; i < 50; i++) {
    digitalWrite(LEDS, HIGH);
    delay(1000 / 30);
    digitalWrite(LEDS, LOW);
    delay(1000 / 30);
  }
  findMode();
}

//Variable timer function -> Set boolean to false after timer runs out. Non blocking code. (Button to cancel)
void findMode() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(BUTTONLED, HIGH);
    delay(SRD * 1000);
    digitalWrite(BUTTONLED, LOW);
    delay(SRD * 1000);
  }
  digitalWrite(FET, LOW);
  delay(2000);
  checkButton();
}

void checkButton() {

  sensorValue = digitalRead(BUTTONVALUE);

  if(sensorValue == HIGH){
    digitalWrite(BUTTONLED, HIGH);
    if (start == true) {
      start = false;
    }
  }else{
    digitalWrite(BUTTONLED, LOW);
  }

  
  //    unsigned long currentMillis = millis();
//  sensorValue = analogRead(2); //Read p3
//  if (sensorValue < oldSensorValue - 50) {
//    oldSensorValue = sensorValue;
//    lowestV = sensorValue;
//  } else if (sensorValue > oldSensorValue + 50) {
//    oldSensorValue = sensorValue;
//    highestV = sensorValue;
//    if (start == true) {
//      start = false;
//      //      flash();
//    }
//  }


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

  } else if (sensorValue == LOW) {
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
  if (flashActivated == true && button == false) {
    flash();
  }

}
