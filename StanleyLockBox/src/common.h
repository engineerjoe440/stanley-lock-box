/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2023
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>
#include <Servo.h>

// Constant Pin Definitions
#define ledPin 13
#define solenoidPin 12
#define servoPin 9
#define knockPin 0
#define binary1Pin 24
#define binary2Pin 25
#define binary4Pin 22
#define binary8Pin 23
#define morseBeepPin 49
#define CW_SPEED 15
#define HEADING_TOLERANCE 15
#define MORSE_PERIOD 20 // seconds

const String keyCode = "1B5";
const char morsePrompt[] = "two";
const char morseExpectation = '2';

/********************* OTHER FUNCTIONS ****************************************/
void unlock(void);
void flashLED(void);
uint8_t readBinarySwitches(void);

