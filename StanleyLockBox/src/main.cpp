/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2023
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>
#include <Servo.h>
#include <TM1637.h>
#include <Wire.h>
#include <cww_MorseTx.h>
#include <DFRobot_QMC5883.h>
#include "common.h"
#include "keypad.h"
#include "fsm.h"

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, KEYPAD_ROWS, KEYPAD_COLUMNS );

byte knockThreshold = 200;

Servo knockServo;

// Instantiation and pins configurations
TM1637 ledSegment(ledSegmentDIO, ledSegmentCLK);

DFRobot_QMC5883 compass(&Wire, /*I2C addr*/ QMC5883_ADDRESS);

cww_MorseTx morse(morseBeepPin, CW_SPEED);

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

float evaluateHeading(void) {
  sVector_t mag = compass.readRaw();
  compass.getHeadingDegrees();
  Serial.print("Heading (degrees): ");
  Serial.println(mag.HeadingDegress);

  return mag.HeadingDegress;
}

bool caseState_keypad(void) {
  // Handle the Keypad Operations and Move to Next State When Appropriate
  static String inputCode = "   "; // Start with three spaces as empty characters
  static bool reserved = false;
  bool result = false;
  char key = keypad.getKey();

  if (!reserved) {
    reserved = true;
    // Make sure there's PLENTY of space... just because
    inputCode.reserve(32);
  }

  // Update key-press queue when key is valid
  if (key) {
    // Trim off the first Character and add new key
    inputCode = inputCode.substring(1, 3) + key;
    Serial.println(inputCode);

    // Validate Input Code
    if (inputCode == keyCode) {
      // Move to Next State
      result = true;
      Serial.println("Key Pin Passed!");
    }
  }

  return result;
}

bool caseState_binaryCode(uint8_t newBinary, uint8_t lastBinary) {
  // Handle the Binary Switches Input and Move to Next State When Appropriate
  bool result = false;

  // Display First Binary Code
  ledSegment.display("4", false, false);
  do {
    newBinary = readBinarySwitches();
  } while (newBinary != 4);
  Serial.println("First binary code presented.");
  // Display Second Binary Code
  ledSegment.display("13", false, false);
  do {
    newBinary = readBinarySwitches();
  } while (newBinary != 13);
  Serial.println("Second binary code presented.");
  result = true;
  Serial.println("Binary Input Passed!");
  ledSegment.clearScreen();

  return result;
}

bool caseState_morse(uint32_t elapsedMs) {
  // Send the Morse Code Message to Issue the Directional Message
  bool result = false;
  static uint32_t count = 0;


  if (count == 0) {
    // Set the Count to its Maximum Period
    count = MORSE_PERIOD * 1000;

    // Indicate we're not "listening"
    digitalWrite(ledPin, LOW);
    morse.send(morsePrompt);
    digitalWrite(ledPin, HIGH);
  }
  if (count > elapsedMs) {
    count -= elapsedMs;
    Serial.println(count);
  } else {
    count = 0;
  }
  

  result = keypad.getKey() == morseExpectation;


  return result;
}

bool caseState_compass(void) {
  // Evaluate the Heading of the Box, Look for North-Bearing
  static float headings[] = {90, 90, 90, 90, 90, 90, 90};
  bool result = true;
  int i = 0;

  for (i = 0; i < 5; i++) {
    headings[i] = headings[i+1];
  }
  headings[5] = evaluateHeading();
  for (i = 0; i < 6; i++) {
    result = result && bool(abs(headings[i]) < HEADING_TOLERANCE);
  }

  return result;
}

bool caseState_knock(void) {
  // "Listen" for the Secret Knock
  static bool knockPromptCompleted = false;
  static uint32_t knockResetThresholdPeriod = 0;
  static uint8_t knockCount = 0;
  bool result = false;
  byte knockMx = analogRead(knockPin);

  
  // Play Leading Portion of "Shave and a Haircut"
  if (!knockPromptCompleted) {
    promptKnock();
    knockCount = 0; // Reset
    Serial.println("Waiting for Knock...");
    knockPromptCompleted = true;
  }

  // When Keypad Passed, Count Knocks
  if (knockMx >= knockThreshold){
    Serial.println("Knock.");
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
    result = knockCount >= 2;
  }

  // Update Threshold On Specific Period After Last Knock
  if (!result) {
    if (knockResetThresholdPeriod == 1) {
      // Determine a *NEW* Baseline Threshold
      setThreshold();
      knockResetThresholdPeriod = 0;
    } else if (knockResetThresholdPeriod > 0) {
      // Decrement Counter
      knockResetThresholdPeriod--;
    }
  }

  return result;
}

void setup() {
  /*****************************************************************************
   * Setup Function:
   * Prepare the System.
   ****************************************************************************/
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
  ledSegment.init();           // Initializes the display
  ledSegment.setBrightness(3); // Set brightness to level 3
  // Setup serial port
  Serial.begin(9600);
  // Initialize Device
  Serial.println("Initializing I2C devices...");
  /* Initialise the sensor */
  while (!compass.begin())
  {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }
  evaluateHeading();
  // Determine a Baseline Threshold
  setThreshold();
  // Set LED Segment
  ledSegment.display("go", false, false, 2);
  // Flash LED
  flashLED();
  ledSegment.clearScreen();
}

void loop() {
  /*****************************************************************************
   * Main Function:
   * Iteratively accept key inputs, validate them against the static `keyCode`.
   * Iteratively "listens" for knock sensor input.
   ****************************************************************************/
  static lock_stage state = INIT;
  static uint8_t lastBinary, newBinary;
  static long now, lastTime;
  lock_stage nextState;
  long timeDelta;

  // Read Binary Inputs, Always
  if ((newBinary = readBinarySwitches()) != lastBinary) {
    lastBinary = newBinary;
    Serial.print("Binary Value: ");
    Serial.println(newBinary);
  }

  now = millis();
  timeDelta = now - lastTime;
  
  //---------- FINITE STATE MACHINE
  switch (state) {

    case INIT:
      nextState = KEYPAD;
      break;
    
    case KEYPAD:
      if (caseState_keypad()) {
        nextState = BINARY_CODE;
      }
      break;
    
    case BINARY_CODE:
      if (caseState_binaryCode(newBinary, lastBinary)) {
        nextState = MORSE;
      }
      break;
    
    case MORSE:
      if (caseState_morse((uint32_t)timeDelta)) {
        nextState = COMPASS;
      }
      break;
    
    case COMPASS:
      if (caseState_compass()) {
        nextState = KNOCK;
      }
      break;
    
    case KNOCK:
      if (caseState_knock()) {
        nextState = UNLOCK;
      }
      break;
  
    case UNLOCK:
     unlock();
     nextState = COMPLETE;
     break;
    

    case COMPLETE:
      // Do Nothing
      break;

  }
  //----------- END FINITE STATE MACHINE

  if (state != nextState) {
    Serial.println("Advancing Lock Stage!");
    flashLED();
  }

  lastTime = now;
  state = nextState;
}