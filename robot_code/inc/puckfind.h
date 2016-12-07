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

int seesPuck(void);
void setDriveToPuck(void);
void setAmbient(void);
int hasPuck(void);


#endif