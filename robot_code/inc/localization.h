// -----------------------------------------------------------------------------
// Header File for Localization Code
// -----------------------------------------------------------------------------

#ifndef localization__
#define localization__

#include <stdio.h>
#include <math.h>
#include "m_wii.h"

/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

char loc_readWii();
// FUNCTIONALITY: Reads the data from the Wii and determines position values
// TAKES: nothing
// RETURNS: 0 if error connecting to Wii. 1 if successful connection

unsigned int loc_getX();
// FUNCTIONALITY: Returns global X positioning value
// TAKES: nothing
// RETURNS: global X position in cm. Far Left: 0, Far Right: 230

unsigned int loc_getY();
// FUNCTIONALITY: Returns global Y positioning value
// TAKES: nothing
// RETURNS: global Y position in cm. Bottom: 0, Top: 120

unsigned int loc_getT();
// FUNCTIONALITY: Returns global orientation value
// TAKES: nothing
// RETURNS: 100 * the angle from the positive x axis in radians

char loc_getSide();
// FUNCTIONALITY: Determines side of the field, used for initial positioning
// TAKES: nothing
// RETURNS: 0 if no data, 1 if left side, 2 if right side

#endif