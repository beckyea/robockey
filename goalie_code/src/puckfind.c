#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "puckfind.h"
#include "vals.h"
#include "drive.h"

#define NUM_PTS 4
#define CLOSE_THRESHOLD 1500  // CHANGE THIS USING SCALED FACTOR

volatile int ADC_Flag = 0;
enum PT { Left = 2, FrontLeft = 1, FrontRight = 0, Right = 3 };
int PTs [4] = { 0, 0, 0, 0 }; //PT ADC values
int ADC_Check = 0; //incr through pt channels

int ptNoise = 0; // ambient noise
int maxPTval = 1023; // maximum PT reading after removing ambient
int MAX_THRESHOLD = 1023;

void printValues(void);
void normalizePTs(void);
double approxAngle(double range, int val1, int val2);

void printValues(void) {
	m_usb_tx_string("\nL: "); m_usb_tx_int(PTs[0]);
	m_usb_tx_string("\tFL: ");  m_usb_tx_int(PTs[1]);
	m_usb_tx_string("\tFR: ");  m_usb_tx_int(PTs[2]);
	m_usb_tx_string("\tR: ");  m_usb_tx_int(PTs[3]);
}

// returns whether values were found
int puck_getADCValues(void) {
	if (ADC_Flag != 0) {  //If ADCs are being read
		clear(ADCSRA,ADEN); // Disable ADC
		printValues(); // COMMENT THIS LINE IN FINAL VERSION
		ADC_Flag = 0;
		set(ADCSRA,ADEN); // Re-enable ADC
		set(ADCSRA,ADSC); // Start next conversion
		return 1;
	}
	return 0;
}

void puck_drive(void) {
	normalizePTs();
	rangeVal = (PTs[Left] + PTs[FrontLeft] + PTs[FrontRight] + PTs[Right]) / 4;
	if (rangeVal < 0.10) {
		clear(PORTB, 0);
		gameState = PATROL;
	} else if (abs(PTs[FrontRight] - PTs[FrontLeft] < 10)) { 
		// assume puck is centered in front of the goalie bot
		stop(); set(PORTB, 0);
	} else if (PTs[Left] >= MAX_THRESHOLD || PTs[Left] > PTs[Right]) { 
		gameState = SEES_LEFT; set(PORTB, 0);
	} else if (PTs[Right] >= MAX_THRESHOLD || PTs[Right] > PTs[Left]) { 
		gameState = SEES_RIGHT;set(PORTB, 0);
	}
}

// normalizes PTs by 
void normalizePTs(void) {
	int i;
	for (i = 0; i < NUM_PTS; i++) { // SUBTRACT 1 from NUM_PTS IN REAL VERSION
		if (PTs[i] - ptNoise < 0) { PTs[i] = 0; }
		else { PTs[i] = PTs[i] - ptNoise; }
	}
}

void setAmbient(void) {
	while (!puck_getADCValues());
	ptNoise = PTs[Left] < PTs[Right]  ?  PTs[Left] : PTs[Right];
	maxPTval = 1023 - ptNoise;
	MAX_THRESHOLD = maxPTval;
}

ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN); //Disable ADC subsystem
	PTs[ADC_Check] = (unsigned int) ADC; //Assign current ADC value to channel in loop
	if (ADC_Check == NUM_PTS - 1) {   //If all channels read, reset loop throuch ADC channels
		ADC_Check = 0; 
		ADC_Flag = 1; 
	}
	else ADC_Check++;

	switch(ADC_Check){ //Switch case to iterate through phototransistors
		case 0: // Set ADC to F0
		clear(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 1: // Set ADC to F1
		clear(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 2: // Set ADC to F4
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 3: // Set ADC to F5
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
	}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC); //start next conversion
}