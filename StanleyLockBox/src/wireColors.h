/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2025
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>

enum wireColor_t {
    NONE,
    GREEN,
    RED,
    WHITE
};

extern const char* wireColorNames[];

bool isWireCut(wireColor_t desiredCut);
wireColor_t selectCutWire();