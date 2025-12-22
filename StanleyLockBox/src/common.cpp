/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2025
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include "common.h"

void unlock() {
  Serial.println("Unlocking Box!");
  // Perform the unlocking operation!
  digitalWrite(solenoidPin, HIGH);
  delay(UNLATCH_PERIOD * 1000); // Delay 60 seconds before locking again
  digitalWrite(solenoidPin, LOW);
  Serial.println("Releasing Latch.");
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

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Set the RGB LED to the Specified Color
  analogWrite(rgbLEDPinR, r);
  analogWrite(rgbLEDPinG, g);
  analogWrite(rgbLEDPinB, b);
}

void setRGBHex(uint32_t hexColor) {
  // Set the RGB LED to the Specified Hex Color
  uint8_t r = (hexColor >> 16) & 0xFF;
  uint8_t g = (hexColor >> 8) & 0xFF;
  uint8_t b = hexColor & 0xFF;
  setRGB(r, g, b);
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

bool timedOut(uint32_t numMillis, uint32_t &startTime) {
  // Run Until Timeout Period has Elapsed
  uint32_t currentTime = millis();
  if (startTime == 0) {
    startTime = currentTime;
  }
  return (currentTime - startTime) >= numMillis;
}

bool inRange(uint16_t value, uint16_t target, uint16_t tolerance) {
  // Check if a Value is within a Target +/- Tolerance
  return (value >= (target - tolerance)) && (value <= (target + tolerance));
}
