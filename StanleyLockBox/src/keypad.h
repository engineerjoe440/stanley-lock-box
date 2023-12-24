/*******************************************************************************
 * StanleyLockBox
 * Joe Stanley | Stanley Solutions | 2023
 ******************************************************************************/

#include <Arduino.h>
#include <stdio.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLUMNS 4

#ifndef keypad_keys
#define keypad_keys 1
// Define the symbols on the buttons of the keypads
char keys[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};

byte pin_rows[KEYPAD_ROWS] = {3, 2, 8, 0}; //connect to the row pinouts of the keypad
byte pin_column[KEYPAD_COLUMNS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad
#endif
