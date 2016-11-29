// -----------------------------------------------------------------------------
// Header File for Puck Finding Code
// -----------------------------------------------------------------------------

#ifndef puckfind__
#define puckfind__

#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"

/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

void puck_getADCValues(void);
// FUCTION: Finds the puck position based on ADC Input Values


#endif