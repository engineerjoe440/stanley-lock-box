/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2022
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>
#include <Servo.h>
#include <TM1637.h>

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

byte knockThreshold = 200;
uint8_t knockCount = 0;
uint32_t knockResetThresholdPeriod = 0;
bool keyPadPassed = false;
bool binaryInPassed = false;

Servo knockServo;

// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 ledsegment(52, 53);

// Constant Pin Definitions
const uint8_t ledPin = 13;
const uint8_t solenoidPin = 12;
const uint8_t servoPin = 9;
const uint8_t knockPin = 0;
const uint8_t binary1Pin = 24;
const uint8_t binary2Pin = 25;
const uint8_t binary4Pin = 22;
const uint8_t binary8Pin = 23;

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

void setThreshold() {
  const uint8_t num_cnt = 100;
  uint32_t overall = 0;
  uint32_t max_mx  = 0;
  byte mx;
  for (int i = 0; i < num_cnt; i++) {
    mx = analogRead(knockPin);
    overall += mx;
    max_mx = max(max_mx, mx);
  }
  knockThreshold = max(knockThreshold, (overall / num_cnt)); // Average
  knockThreshold *= 1.20; // 120% of Average
  knockThreshold = max(knockThreshold, max_mx+4); // Ensure Larger than Max
  char buffer[255];
  sprintf(buffer, "Knock Threshold: %d", knockThreshold);
  Serial.println(buffer);
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

uint8_t readBinarySwitches() {
  // Read Switches
  uint8_t result = 0;
  if (!digitalRead(binary1Pin)) { result += 1; }
  if (!digitalRead(binary2Pin)) { result += 2; }
  if (!digitalRead(binary4Pin)) { result += 4; }
  if (!digitalRead(binary8Pin)) { result += 8; }
  return result;
}

void setup() {
  // Setup Pins
  pinMode(ledPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(binary1Pin, INPUT);
  pinMode(binary2Pin, INPUT);
  pinMode(binary4Pin, INPUT);
  pinMode(binary8Pin, INPUT);
  // Setup Servo
  knockServo.attach(servoPin);
  knockServo.write(0);
  ledsegment.init();           // Initializes the display
  ledsegment.setBrightness(3); // Set brightness to level 3
  // Setup serial port
  Serial.begin(9600);
  // Make sure there's PLENTY of space... just because
  inputCode.reserve(32);
  // Determine a Baseline Threshold
  setThreshold();
  // Set LED Segment
  ledsegment.display("go", false, false, 2);
  // Flash LED
  flashLED();
  ledsegment.clearScreen();
}

void loop() {
  /*****************************************************************************
   * Main Function:
   * Iteratively accept key inputs, validate them against the static `keyCode`.
   * Iteratively "listens" for knock sensor input.
   ****************************************************************************/
  char key = keypad.getKey();
  byte knockMx = analogRead(knockPin);
  static uint8_t lastBinary, newBinary;
  
  newBinary = readBinarySwitches();
  if (newBinary != lastBinary) {
    lastBinary = newBinary;
    Serial.print("Binary Value: ");
    Serial.println(newBinary);
  }

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
    }
  }

  if (keyPadPassed && !binaryInPassed) {
    // Display First Binary Code
    ledsegment.display("4", false, false);
    do {
      newBinary = readBinarySwitches();
    } while (newBinary != 4);
    Serial.println("First binary code presented.");
    // Display Second Binary Code
    ledsegment.display("13", false, false);
    do {
      newBinary = readBinarySwitches();
    } while (newBinary != 13);
    Serial.println("Second binary code presented.");
    binaryInPassed = true;
    Serial.println("Binary Input Passed!");
    ledsegment.clearScreen();
    // Flash LED
    flashLED();
    // Play Leading Portion of "Shave and a Haircut"
    promptKnock();
    knockCount = 0; // Reset
    Serial.println("Waiting for Knock...");
  }

  // When Keypad Passed, Count Knocks
  if (knockMx >= knockThreshold){
    if (keyPadPassed && binaryInPassed) {
      knockCount++;
      Serial.println(knockCount);
      digitalWrite(ledPin, LOW);
      delay(200); // ms
      digitalWrite(ledPin, HIGH);
      // Determine a *NEW* Baseline Threshold
      setThreshold();
      // Negative Overflow to Access Maximum Value
      knockResetThresholdPeriod = 0 - 1;

      // Validate Knock Count
      if (knockCount >= 2) {
        unlock();
      }
    } else {
      setThreshold();
      Serial.println("Knock.");
    }
  }

  // Update Threshold On Specific Period After Last Knock
  if (knockResetThresholdPeriod == 1) {
    // Determine a *NEW* Baseline Threshold
    setThreshold();
    knockResetThresholdPeriod = 0;
  } else if (knockResetThresholdPeriod > 0) {
    // Decrement Counter
    knockResetThresholdPeriod--;
  }
}