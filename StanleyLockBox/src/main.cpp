/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2021
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>

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

const uint8_t ledPin = 13;
const uint8_t solenoidPin = 12;

void setup() {
  // Setup Pins
  pinMode(ledPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  // Setup serial port
  Serial.begin(9600);
  // Make sure there's PLENTY of space... just because
  inputCode.reserve(32);
  // Flash LED
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100); // milliseconds
    digitalWrite(ledPin, LOW);
    delay(100); // milliseconds
  }
  digitalWrite(ledPin, HIGH); // Leave On!
}

void loop() {
  /*****************************************************************************
   * Main Function:
   * Iteratively accept key inputs, validate them against the static `keyCode`.
   ****************************************************************************/
  char key = keypad.getKey();
  uint timeCounter = 0;

  // Update key-press queue when key is valid
  if (key) {
    // Trim off the first Character and add new key
    inputCode = inputCode.substring(1, 2) + key;
    Serial.println(inputCode);

    // Validate Input Code
    if (inputCode == keyCode) {
      // Unlock the Box!
      Serial.println("Unlocking box!");
      unlock();
    }
  }
}

void unlock() {
  // Perform the unlocking operation!
  digitalWrite(solenoidPin, HIGH);
  delay(60 * 1000); // Delay 60 seconds before locking again
  digitalWrite(solenoidPin, LOW);
}