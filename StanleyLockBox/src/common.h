/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2025
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include <Keypad.h>
#include <Servo.h>

// Constant Pin Definitions
#define randomSeedPin A0
#define ledPin A5
#define solenoidPin 63
#define servoPin A1
#define knockPin A3
#define binary1Pin 43
#define binary2Pin 49
#define binary4Pin 45
#define binary8Pin 47
#define morseBeepPin 32
#define ledSegmentDIO 51
#define ledSegmentCLK 53
#define rgbLEDPinR 9
#define rgbLEDPinG 11
#define rgbLEDPinB 10
#define redWirePin 39
#define greenWirePin 35
#define whiteWirePin 37

// Other Definitions
#define CW_SPEED 15
#define HEADING_TOLERANCE 20
#define MORSE_PERIOD 20 // seconds
#define UNLATCH_PERIOD 20UL // seconds

// Static Colors
const uint32_t hanPurple = 0x5218fa; // Han Purple #5218fa | rgb(82,24,250)
const uint8_t hanPurple_r = 82;
const uint8_t hanPurple_g = 24;
const uint8_t hanPurple_b = 250;

const String keyCode = "1B5";
const char morseExpectation = '2';
const uint32_t rgbEntryDelay = 2000; // ms
const uint32_t watchdogTimeout = (uint32_t)60*(uint32_t)1000; // ms

/********************* OTHER FUNCTIONS ****************************************/
void unlock(void);
void flashLED(void);
void setRGB(uint8_t r, uint8_t g, uint8_t b);
void setRGBHex(uint32_t hexColor);
uint8_t readBinarySwitches(void);
bool timedOut(uint32_t numMillis, uint32_t &startTime);
bool inRange(uint16_t value, uint16_t target, uint16_t tolerance);
