// -----------------------------------------------------------------------------
// Header File for Initialization Code
// -----------------------------------------------------------------------------

#ifndef initialization__
#define initialization__
#include "vals.h"
/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

void init_all (void); // calls all other initialization commands in the localization.c file
void init_mwii(void); // initializes the mWii
void init_mrf(void); // initializes the mRF sensor
void init_driver(void); // initializes the pins for motor control
void init_adc(void);
void init_setSide(int i); // sets the coordinates of both goals as global variables
void init_setRobot(void); // initializes the values and strategy of the bot

#endif