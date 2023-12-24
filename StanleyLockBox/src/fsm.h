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
