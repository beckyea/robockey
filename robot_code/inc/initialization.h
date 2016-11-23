// -----------------------------------------------------------------------------
// Header File for Initialization Code
// -----------------------------------------------------------------------------

#ifndef initialization__
#define initialization__

#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"

/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

void init_all (void); 
// FUNCTION: Calls all other initialization commands in the localization.c file

void init_mwii(void);
// FUNCTION: Initializes the mWii

void init_mrf(void);
// FUNCTION: Initializes the mRF sensor

void init_driver(void);
// FUNCTION: Initializes the pins for motor control

#endif