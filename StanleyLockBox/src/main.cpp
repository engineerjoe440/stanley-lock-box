/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2021
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};

// TODO: Verify these pinouts
byte pin_rows[ROWS] = {3, 2, 8, 0}; //connect to the row pinouts of the keypad
byte pin_column[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROWS, COLS );

const String keyCode = "1B5";
String inputCode = "   "; // Start with three spaces as empty characters

const byte knockThreshold = 200;
uint8_t knockCount = 0;

Servo knockServo;

// Constant Pin Definitions
const uint8_t ledPin = 13;
const uint8_t solenoidPin = 12;
const uint8_t servoPin = 9;
const uint8_t knockPin = 0;

void unlock() {
  Serial.println("Unlocking Box!");
  // Perform the unlocking operation!
  knockServo.write(0);
  digitalWrite(solenoidPin, HIGH);
  delay(60UL * 1000); // Delay 60 seconds before locking again
  digitalWrite(solenoidPin, LOW);
}

void promptKnock() {
  // "Play" a Knock Sound on the Servo
  knockServo.write(90);
  delay(400); // ms
  knockServo.write(0);
  delay(250); // ms
  knockServo.write(90);
  delay(250); // ms
  knockServo.write(0);
  delay(400); // ms
  knockServo.write(90);
}

void flashLED() {
  // Flash LED
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100); // milliseconds
    digitalWrite(ledPin, LOW);
    delay(100); // milliseconds
  }
  digitalWrite(ledPin, HIGH); // Leave On!
}

void setup() {
  // Setup Pins
  pinMode(ledPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  // Setup Servo
  knockServo.attach(servoPin);
  knockServo.write(0);
  // Setup serial port
  Serial.begin(9600);
  // Make sure there's PLENTY of space... just because
  inputCode.reserve(32);
  // Flash LED
  flashLED();
}

void loop() {
  /*****************************************************************************
   * Main Function:
   * Iteratively accept key inputs, validate them against the static `keyCode`.
   * Iteratively "listens" for knock sensor input.
   ****************************************************************************/
  char key = keypad.getKey();
  bool knockDetected = (analogRead(knockPin) >= knockThreshold);
  bool keyPadPassed = false;
  Serial.println(knockDetected);

  // Update key-press queue when key is valid
  if (key) {
    // Trim off the first Character and add new key
    inputCode = inputCode.substring(1, 3) + key;
    Serial.println(inputCode);

    // Validate Input Code
    if (inputCode == keyCode) {
      // Unlock the Box!
      keyPadPassed = true;
      Serial.println("Key Pin Passed!");
      // Flash LED
      flashLED();
      // Play Leading Portion of "Shave and a Haircut"
      promptKnock();
      knockCount = 0; // Reset
      Serial.println("Waiting for Knock...");
    }
  }

  // When Keypad Passed, Count Knocks
  if (knockDetected && keyPadPassed) {
    knockCount++;
    Serial.println(knockCount);

    // Validate Knock Count
    if (knockCount == 2) {
      unlock();
    }
  }
}