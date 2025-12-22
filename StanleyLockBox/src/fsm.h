/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2023
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>

enum lock_stage {
    INIT,
    KEYPAD,
    BINARY_CODE,
    MORSE,
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
    IDNAME(COMPASS),
    IDNAME(KNOCK),
    IDNAME(UNLOCK),
    IDNAME(COMPLETE)
};
