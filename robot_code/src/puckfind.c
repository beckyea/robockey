#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "puckfind.h"
#include "vals.h"

#define NUM_PTS 8
#define CLOSE_THRESHOLD 1500  // CHANGE THIS USING SCALED FACTOR

volatile int ADC_Flag = 0;
enum PT { TopRight = 0, Right = 1, Back = 2, Left = 3, TopLeft = 4, InnerLeft = 5, InnerRight = 6, Down = 7 };
int PTs [8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //PT ADC values
int ADC_Check = 0; //incr through pt channels

int ptNoise = 0; // ambient noise
int maxPTval = 1023; // maximum PT reading after removing ambient
int MAX_THRESHOLD = 1023;

void printValues(void);
void puck_findAngle(void);
void normalizePTs(void);
double approxAngle(double range, int val1, int val2);

void printValues(void) {
	m_usb_tx_string("\nTR: "); m_usb_tx_int(PTs[0]);
	m_usb_tx_string("\tR: ");  m_usb_tx_int(PTs[1]);
	m_usb_tx_string("\tB: ");  m_usb_tx_int(PTs[2]);
	m_usb_tx_string("\tL: ");  m_usb_tx_int(PTs[3]);
	m_usb_tx_string("\tTL: "); m_usb_tx_int(PTs[4]);
	m_usb_tx_string("\tIL: "); m_usb_tx_int(PTs[5]);
	m_usb_tx_string("\tIR: "); m_usb_tx_int(PTs[6]);
	m_usb_tx_string("\tD: ");  m_usb_tx_int(PTs[7]);
	m_usb_tx_string("\tAngle: "); m_usb_tx_int(puckAngle*180/3.1416);
	m_usb_tx_string("\tDist: "); m_usb_tx_int(rangeVal * 100);
	m_usb_tx_string("\tAmbient: "); m_usb_tx_int(ptNoise);
}

// returns whether values were found
int puck_getADCValues(void) {
	if (ADC_Flag != 0) {  //If ADCs are being read
		clear(ADCSRA,ADEN); // Disable ADC
		puck_findAngle();
		printValues(); // COMMENT THIS LINE IN FINAL VERSION
		ADC_Flag = 0;
		set(ADCSRA,ADEN); // Re-enable ADC
		set(ADCSRA,ADSC); // Start next conversion
		return 1;
	}
	return 0;
}

void puck_findAngle(void) {
	normalizePTs();
	rangeVal = (PTs[TopRight] + PTs[TopLeft] + PTs[InnerLeft] + PTs[InnerRight]) / 4;
	rangeVal = (rangeVal + PTs[Right] + PTs[Back] + PTs[Left]) / (4 * maxPTval);
	if (rangeVal < 0.10) {
		clear(PORTB, 0);
		gameState = PATROL;
	} else { 
		gameState = GO_TO_PUCK;
		set(PORTB, 0);
		if (PTs[Down] > MAX_THRESHOLD && PTs[InnerLeft] > MAX_THRESHOLD && PTs[InnerRight] > MAX_THRESHOLD) {
				puckAngle = 0;
				gameState = GO_TO_GOAL;
		} else if (PTs[TopLeft] > MAX_THRESHOLD && PTs[TopRight] > MAX_THRESHOLD && PTs[InnerLeft] > MAX_THRESHOLD && PTs[InnerRight] > MAX_THRESHOLD) { 
			puckAngle = 0; // puck is straight ahead
		} else if ((PTs[TopRight] > MAX_THRESHOLD && PTs[InnerLeft] > MAX_THRESHOLD && PTs[InnerRight] > MAX_THRESHOLD) ||
				   (PTs[TopLeft] > MAX_THRESHOLD && PTs[TopRight] > MAX_THRESHOLD && PTs[InnerRight] > MAX_THRESHOLD)) {
			puckAngle = 0.1; // some small angle to the left -- TODO: FIGURE OUT HOW TO CALCULATE THIS
		} else if ((PTs[TopLeft] > MAX_THRESHOLD && PTs[InnerLeft] > MAX_THRESHOLD && PTs[InnerRight] > MAX_THRESHOLD) ||
				   (PTs[TopLeft] > MAX_THRESHOLD && PTs[TopRight] > MAX_THRESHOLD && PTs[InnerLeft] > MAX_THRESHOLD)) {
			puckAngle = 6.18; // some small angle to the right - TODO: FIGURE OUT HOW TO CALCULATE THIS
		} else if (rangeVal > .10) {
			// find 2 max PTs
			int i, maxPT1, maxPT2; maxPT1 = 0; maxPT2 = 1; 
			for (i = 1; i < NUM_PTS - 1; i++) { // CHANGE THIS
				if (PTs[i] > PTs[maxPT1]) { maxPT2 = maxPT1; maxPT1 = i; }
				else if (PTs[i] > PTs[maxPT2]) { maxPT2 = i; }
			}
			// if the indices are continuous
			if (maxPT1 - maxPT2 == 1 || maxPT2 - maxPT1 == 1 || maxPT1 - maxPT2 == 6 || maxPT2 - maxPT1 == 6) {
				int smallerIndex = maxPT1 < maxPT2 ? maxPT1 : maxPT2;
				if (smallerIndex == 0 && (maxPT1 == 6 || maxPT2 == 6)) { smallerIndex = 6; }
				switch (smallerIndex) {
					case TopRight : puckAngle = 5.410 - approxAngle(0.698, PTs[TopRight], PTs[Right]); break;
					case Right :    puckAngle = 4.712 - approxAngle(1.571, PTs[Right], PTs[Back]);     break;
					case Back :     puckAngle = 3.142 - approxAngle(1.571, PTs[Back], PTs[Left]);      break;
					case Left :     puckAngle = 1.571 - approxAngle(0.698, PTs[Left], PTs[TopLeft]);   break;
					default :       puckAngle = 0; break; // TODO: WHAT DO WE DO HERE???
				}
			}
			// if the indices are not continuous, assume that it is angled in the direction of the largest value
			else {
				switch (maxPT1) {
					case TopRight : puckAngle = 5.41052; break;
					case Right :    puckAngle = 2 * 3.1416/2; break;
					case Back :     puckAngle = 3.1416; break;
					case Left :     puckAngle = 3.1416/2; break;
					case TopLeft :  puckAngle = 0.872665; break;
					default :       puckAngle = 0; break;
				}
			}
		}
	}
}

double approxAngle(double range, int val1, int val2) {
	return range * ((double) (val1) / (val1 + val2));
}

// normalizes PTs by 
void normalizePTs(void) {
	int i;
	for (i = 0; i < NUM_PTS - 1; i++) { // SUBTRACT 1 from NUM_PTS IN REAL VERSION
		if (PTs[i] - ptNoise < 0) { PTs[i] = 0; }
		else { PTs[i] = PTs[i] - ptNoise; }
	}
}

void setAmbient(void) {
	while (!puck_getADCValues());
	ptNoise = PTs[Back] < PTs[Left]  ?  PTs[Back] : PTs[Left];
	ptNoise = ptNoise   < PTs[Right] ?  ptNoise	  : PTs[Right];
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
	else ADC_Check ++;

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
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		default:
		break;
	}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC); //start next conversion
}