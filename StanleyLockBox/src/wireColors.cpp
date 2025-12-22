/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2025
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>
#include "common.h"
#include "wireColors.h"

const char* wireColorNames[] = {
    "NONE",
    "GREEN",
    "RED",
    "WHITE"
};

bool isWireCut(wireColor_t desiredCut) {
  // Check if the Desired Wire has been Cut
  bool result = false;
  switch (desiredCut) {
    case GREEN:
      result = digitalRead(greenWirePin) == HIGH;
      break;
    case RED:
      result = digitalRead(redWirePin) == HIGH;
      break;
    case WHITE:
      result = digitalRead(whiteWirePin) == HIGH;
      break;
    default:
      result = false;
      break;
  }
  return result;
}

wireColor_t selectCutWire() {
  wireColor_t result;
  // Randomly Select a Wire Color to Cut
  randomSeed(analogRead(randomSeedPin) + millis()); // Seed Random Number Generator
  uint8_t selection = random(0, 3); // 0 to 2
  switch (selection) {
    case 0:
      result = GREEN;
    case 1:
      result = RED;
    case 2:
      result = WHITE;
  }
  Serial.print("Selected Wire to Cut: ");
  Serial.println(wireColorNames[result]);
  return result;
}