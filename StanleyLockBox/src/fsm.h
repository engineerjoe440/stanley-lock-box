/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2025
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>

enum lock_stage {
    INIT,
    KEYPAD,
    BINARY_CODE,
    MORSE,
    RGB_SET,
    COMPASS,
    KNOCK,
    UNLOCK,
    COMPLETE
};
#define IDNAME(name) #name
const char* stateNames[] = {
    IDNAME(INIT),
    IDNAME(KEYPAD),
    IDNAME(BINARY_CODE),
    IDNAME(MORSE),
    IDNAME(RGB_SET),
    IDNAME(COMPASS),
    IDNAME(KNOCK),
    IDNAME(UNLOCK),
    IDNAME(COMPLETE)
};
