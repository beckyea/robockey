// -----------------------------------------------------------------------------
// Header File for Puck Finding Code
// -----------------------------------------------------------------------------

#ifndef puckfind__
#define puckfind__

/* -----------------------------------------------------------------------------
   Public functions
----------------------------------------------------------------------------- */

int puck_getADCValues(void);
// FUCTION: Finds the puck position based on ADC Input Values
// RETURNS: 1 if values read; 0 if values not read

void setAmbient(void);
void puck_drive(void);

#endif