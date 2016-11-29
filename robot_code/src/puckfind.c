#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "puckfind.h"

#define NUM_PTS 8

volatile int ADC_Flag = 0;
int ADC_Channels[8] = {0,0,0,0,0,0,0,0}; //Array to hold phototransistor ADC values
int ADC_Check=0; //Int to iterate through phototransistor channels

void printValues(void);
void puck_findAngle(void);

void printValues(void) {
	m_usb_tx_string("\nTR: ");
	m_usb_tx_int(ADC_Channels[0]);
	m_usb_tx_string("\tR: ");
	m_usb_tx_int(ADC_Channels[1]);
	m_usb_tx_string("\tB: ");
	m_usb_tx_int(ADC_Channels[2]);
	m_usb_tx_string("\tL: ");
	m_usb_tx_int(ADC_Channels[3]);
	m_usb_tx_string("\tTL: ");
	m_usb_tx_int(ADC_Channels[4]);
	m_usb_tx_string("\tIL: ");
	m_usb_tx_int(ADC_Channels[5]);
	m_usb_tx_string("\tIR: ");
	m_usb_tx_int(ADC_Channels[6]);
	m_usb_tx_string("\tD: ");
	m_usb_tx_int(ADC_Channels[7]);
	m_usb_tx_string("\t");
}

void puck_getADCValues(void) {
	if (ADC_Flag != 0) {  //If ADCs are being read
		clear(ADCSRA,ADEN); // Disable ADC
		printValues(); // UNCOMMENT THIS LINE IN FINAL VERSION
		ADC_Flag = 0;
		set(ADCSRA,ADEN); // Re-enable ADC
		set(ADCSRA,ADSC); // Start next conversion
	}
}

void puck_findAngle(void) {
	//int maxADC1 = 0;
	//int maxADC2 = 0;
	//int z = 0;
	//int maxChannel1 = 0;
	//int maxChannel2 = 0;
    // for (z = 0; z < 8; z++) {                //Find the two phototransistors closest to the puck
    // 	if (ADC_Channels[z]>maxADC1) {
    // 		maxADC2 = maxADC1;         
    // 		maxADC1 = ADC_Channels[z];
    // 		maxChannel1 = z;
    // 	} else if (ADC_Channels[z] > maxADC2) {
    // 		maxADC2 = ADC_Channels[z];
    // 		maxChannel2 = z;
   	// 	}
   	// }
}

ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN); //Disable ADC subsystem
	ADC_Channels[ADC_Check]=ADC; //Assign current ADC value to channel in loop
	if (ADC_Check == NUM_PTS - 1) {   //If all channels read, reset loop throuch ADC channels
		ADC_Check = 0; 
		ADC_Flag = 1; 
	}
	else ADC_Check ++;

	switch(ADC_Check){ //Switch case to iterate through phototransistors
		case 0: // Set ADC to F0
		clear(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 1: // Set ADC to F1
		clear(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 2: // Set ADC to F4
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 3: // Set ADC to F5
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 4: // Set ADC to F6
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 5: // Set ADC to F7
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 6: // Set ADC to D4
		set(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 7: // Set ADC to D6
		set(ADMUX,MUX5);
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