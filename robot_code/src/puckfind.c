#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "puckfind.h"
#include "localization.h"
#include "vals.h"
#include "drive.h"

#define NUM_PTS 8
#define ALPHA 0.4

volatile int ADC_Flag = 0;
enum PT { TopRight = 5, Right = 6, Back = 1, Left = 7, TopLeft = 4, InnerLeft = 2, InnerRight = 3, Down = 0 };
int PTs [8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //PT ADC values
int ADC_Check = 0; //incr through pt channels

int ptNoise = 0; // ambient noise
int maxPTval = 1023; // maximum PT reading after removing ambient
int temp;
int maxPT1, maxPT2;

int hasPuckThreshold = 600;
int noiseThreshold = 20;
int closeThreshold = 200;

void printValues(void);

void printValues(void) {
	m_usb_tx_string("\nTR: "); m_usb_tx_int(PTs[TopRight]);
	m_usb_tx_string("\tR: ");  m_usb_tx_int(PTs[Right]);
	m_usb_tx_string("\tB: ");  m_usb_tx_int(PTs[Back]);
	m_usb_tx_string("\tL: ");  m_usb_tx_int(PTs[Left]);
	m_usb_tx_string("\tTL: "); m_usb_tx_int(PTs[TopLeft]);
	m_usb_tx_string("\tIL: "); m_usb_tx_int(PTs[InnerLeft]);
	m_usb_tx_string("\tIR: "); m_usb_tx_int(PTs[InnerRight]);
	m_usb_tx_string("\tD: ");  m_usb_tx_int(PTs[Down]);
	m_usb_tx_string("\tAmbient: "); m_usb_tx_int(ptNoise);
}

// returns whether values were found
int puck_getADCValues(void) {
	if (ADC_Flag != 0) {  //If ADCs are being read
		clear(ADCSRA,ADEN); // Disable ADC
		ADC_Flag = 0;
		set(ADCSRA,ADEN); // Re-enable ADC
		set(ADCSRA,ADSC); // Start next conversion
		if (time % 10 == 0) { printValues(); }
		return 1;
	}
	return 0;
}

// Returns 1 if the puck is seen; 0 if not
int seesPuck(void) {
	int i; maxPT1 = 0; maxPT2 = 1; 
	for (i = 1; i < NUM_PTS; i++) { 
			if (PTs[i] > PTs[maxPT1]) { maxPT2 = maxPT1; maxPT1 = i; }
			else if (PTs[i] > PTs[maxPT2]) { maxPT2 = i; }
	}
	if (PTs[maxPT1] > noiseThreshold) { set(PORTB, 0); return 1; }
	else { clear(PORTB, 0); return 0; }
}

// Returns 1 if the bot has the puck; 0 if not
int hasPuck(void) { 
	if (PTs[Down] >= hasPuckThreshold && PTs[InnerRight] > 600 && PTs[InnerLeft] > 600) {
		m_green(ON); return 1;
	}
	m_green(OFF) return 0;
}

void setDriveToPuck(void) {
	 // COMMENT THIS LINE IN FINAL VERSION }
	// int i, maxPT1, maxPT2; maxPT1 = 0; maxPT2 = 1; 
	// for (i = 1; i < NUM_PTS; i++) { 
	// 	if (i !=TopLeft && i != TopRight) {
	// 		if (PTs[i] > PTs[maxPT1]) { maxPT2 = maxPT1; maxPT1 = i; }
	// 		else if (PTs[i] > PTs[maxPT2]) { maxPT2 = i; }
	// 	}
	// }
	if (PTs[Back] == PTs[Left]) { maxPT1 = Left; }
	if (PTs[Back] == PTs[Right]) { maxPT1 = Right; }
	// check bounds
	//if (checkInBounds()) {
		// drive to bot
		if (PTs[maxPT1]== TopLeft && (PTs[TopLeft] > PTs[TopRight])) {
			left(); if (time % 10 == 0) {  m_usb_tx_string("left1"); }
		} else if (maxPT1 == TopRight && (PTs[TopRight] > PTs[TopLeft])) {
			right(); if (time % 10 == 0) { m_usb_tx_string("right1");  }
		} else if ((PTs[InnerRight] == PTs[InnerLeft]) && (maxPT1 == InnerLeft || maxPT1 == InnerRight || maxPT2 == InnerLeft || maxPT2 == InnerRight)) {
			fwd_fast(); if (time % 10 == 0) { m_usb_tx_string("fwd1"); }
		} else {
			switch(maxPT1) {
				case Back: right_ip(); if (time % 10 == 0) { m_usb_tx_string("back2");} break;
				case Right: if (PTs[Right] < closeThreshold) { right(); } else { right_ip(); } if (time % 10 == 0) { m_usb_tx_string("right2"); } break;
				case Left: if (PTs[Left] < closeThreshold) { left(); } else { left_ip(); } if (time % 10 == 0) { m_usb_tx_string("left2"); } break;
				case TopLeft: left(); if (time % 10 == 0) { m_usb_tx_string("left3"); } break;
				case TopRight: right();if (time % 10 == 0) { m_usb_tx_string("right3"); } break;
				case InnerLeft: right(); if (time % 10 == 0) { m_usb_tx_string("right4"); } break;
				case InnerRight: left(); if (time % 10 == 0) { m_usb_tx_string("left4"); } break;
				default: fwd_fast(); if (time % 10 == 0) { m_usb_tx_string("fwd3"); } break;
			}
		} checkStuckBot();
	//} else {
	//	goToPoint(0, 0); m_usb_tx_string("other");
	//}
}

// normalizes PTs by 
void normalizePTs(void) {
	int i;
	for (i = 0; i < NUM_PTS; i++) {
		if (PTs[i] - ptNoise < 0) { PTs[i] = 0; }
		else { PTs[i] = PTs[i] - ptNoise; }
	}
}

void setAmbient(void) {
	while (!puck_getADCValues());
	ptNoise = PTs[Back] < PTs[Left]  ?  PTs[Back] : PTs[Left];
	ptNoise = ptNoise   < PTs[Right] ?  ptNoise	  : PTs[Right];
}

ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN); //Disable ADC subsystem
	//temp = PTs[ADC_Check];
	//PTs[ADC_Check] = (unsigned int) ADC - ptNoise; //Assign current ADC value to channel in loop
	//if (PTs[ADC_Check] < 0) { PTs[ADC_Check] = 0; }
	if (ptNoise != 0) { // not first pass - filter the readings
		PTs[ADC_Check] = PTs[ADC_Check] * ALPHA + ((unsigned int) ADC) * (1 - ALPHA) - ptNoise;
		if (PTs[ADC_Check] < 0) { PTs[ADC_Check] = 0; }
	} else { PTs[ADC_Check] = (unsigned int) ADC - ptNoise; 
		if (PTs[ADC_Check] < 0) { PTs[ADC_Check] = 0; }
	}
	if (ADC_Check == NUM_PTS) {   //If all channels read, reset loop throuch ADC channels
		ADC_Check = 0; 
		ADC_Flag = 1; 
	}
	else ADC_Check ++;

	switch(ADC_Check){ //Switch case to iterate through phototransistors
		case 0: // Set ADC to F0
		clear(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 1: // Set ADC to D7
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
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
		case 4: // Set ADC to F6
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 5: // Set ADC to F7
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 6: // Set ADC to D4
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 7: // Set ADC to D6
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		set(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		default:
		break;
	}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC); //start next conversion
}