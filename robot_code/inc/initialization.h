// -----------------------------------------------------------------------------
// Header File for Initialization Code
// -----------------------------------------------------------------------------

#ifndef initialization__
#define initialization__

#include "m_general.h"
#include "vals.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"

/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

void init_all (enum Bot bot); 
// FUNCTION: Calls all other initialization commands in the localization.c file
// INPUT: Bot you're initializing for

void init_mwii(void);
// FUNCTION: Initializes the mWii

void init_mrf(void);
// FUNCTION: Initializes the mRF sensor

void init_driver(void);
// FUNCTION: Initializes the pins for motor control

void init_setRobot(enum Bot bot);
// FUNCTION: Initializes the values and strategy of the bot

#endif