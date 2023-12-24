/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2023
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

uint8_t readBinarySwitches() {
  // Read Switches
  uint8_t result = 0;
  if (!digitalRead(binary1Pin)) { result += 1; }
  if (!digitalRead(binary2Pin)) { result += 2; }
  if (!digitalRead(binary4Pin)) { result += 4; }
  if (!digitalRead(binary8Pin)) { result += 8; }
  return result;
}

