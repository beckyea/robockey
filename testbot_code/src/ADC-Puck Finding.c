
 /* Name: main.c
* Author: <Adnan Jafferjee>
* Copyright: <insert your copyright message here>
* License: <insert your license reference here>
*/
#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"

volatile int ADC_Flag = 0;
// int ADC_Channels[8] = {0,0,0,0,0,0,0,0}; //Array to hold phototransistor ADC values
int ADC_Channels[2] = {0,0}; 
int ADC_Check=0; //Int to iterate through phototransistor channels

//Prototyping subroutines 
void init(void);
void ADC_init(void);
void findDistanceToPuck(void);

int main(void) {
	init();
	ADC_init();
	set(ADCSRA,ADEN); //Enable ADC
	set(ADCSRA,ADSC); // Start conversion
	
	while(1) {
		if (ADC_Flag!=0){  //If ADCs are being read
			m_red(TOGGLE);
			clear(ADCSRA,ADEN); // Disable ADC
			m_usb_tx_string("\nF0:");
			m_usb_tx_int(ADC_Channels[0]);
			m_usb_tx_string("\tF1: ");
			m_usb_tx_int(ADC_Channels[1]);
			findDistanceToPuck();
			// m_usb_tx_string("\tF4: ");
			// m_usb_tx_int(ADC_Channels[2]);
			// m_usb_tx_string("\tF5: ");
			// m_usb_tx_int(ADC_Channels[3]);
			// m_usb_tx_string("\tF6: ");
			// m_usb_tx_int(ADC_Channels[4]);
			// m_usb_tx_string("\tF7: ");
			// m_usb_tx_int(ADC_Channels[5]);
			// m_usb_tx_string("\tD4: ");
			// m_usb_tx_int(ADC_Channels[6]);
			// m_usb_tx_string("\tD6: ");
			// m_usb_tx_int(ADC_Channels[7]);
			// m_usb_tx_string("\t");
			m_wait(200);
			ADC_Flag = 0;
					set(ADCSRA,ADEN); // Re-enable ADC
					set(ADCSRA,ADSC);// Start next conversion
				}
			}
		}

		void init(void) {
	 m_clockdivide(0); // Set system clock prescaler to 1 (16MHz) 
	 m_usb_init(); // Open USB Serial comm.
	 m_disableJTAG(); //turn off JTAG port and allow access to F4-F7 as GPIO
	 sei(); //enable global interrupts
	}

	void ADC_init(void) {
	clear(ADMUX, REFS0); // Make Vcc the reference voltage
	clear(ADMUX, REFS1);
	set(ADCSRA, ADPS2); //Set Clock prescaler to /128 => ADC runs at 125kHz
	set(ADCSRA, ADPS1); 
	set(ADCSRA, ADPS0); 
	//Disable digital on ADC Pins
	set(DIDR0,ADC0D); // F0
	set(DIDR0,ADC1D); // F1
	set(DIDR0,ADC4D); // F4
	set(DIDR0,ADC5D); // F5
	set(DIDR0,ADC6D); // F6
	set(DIDR0,ADC7D); // F7
	set(DIDR2,ADC8D); // D4
	set(DIDR2,ADC9D); // D6
	set(ADCSRA,ADIE); //Enable ADC Interrupt Flag
	clear(ADCSRA,ADATE); // Disable free-running Mode
}

void findDistanceToPuck(void){ 
	int maxADC1 = 0;
	int maxADC2 = 0;
	int z = 0;
	int maxChannel1=0;
	int maxChannel2=0;
    for (z = 0; z<2; z++) {                //Find the two phototransistors closest to the puck
    	if (ADC_Channels[z]>maxADC1) {
    		maxADC2 = maxADC1;         
    		maxADC1 = ADC_Channels[z];
    		maxChannel1 = z;
    	}
    	else if(ADC_Channels[z]>maxADC2){
    		maxADC2 = ADC_Channels[z];
    		maxChannel2 = z;
    	}
    	m_usb_tx_string("\nmaxADC1:");
    	m_usb_tx_int(maxADC1);
    	m_usb_tx_string("\tmaxADC2: ");
    	m_usb_tx_int(maxADC2);
    	m_usb_tx_string("\nmaxChannel1:");
    	m_usb_tx_int(maxChannel1);
    	m_usb_tx_string("\tmaxChannel2: ");
    	m_usb_tx_int(maxChannel2);
    }
}




ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN); //Disable ADC subsystem
	ADC_Channels[ADC_Check]=ADC; //Assign current ADC value to channel in loop
	if(ADC_Check==1){   //If all channels read, reset loop throuch ADC channels
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
	// case 2: // Set ADC to F4
	// clear(ADMUX,MUX5);
	// set(ADMUX,MUX2);
	// clear(ADMUX,MUX1);
	// clear(ADMUX,MUX0);
	// break;
	// case 3: // Set ADC to F5
	// clear(ADMUX,MUX5);
	// set(ADMUX,MUX2);
	// clear(ADMUX,MUX1);
	// set(ADMUX,MUX0);
	// break;
	// case 4: // Set ADC to F6
	// clear(ADMUX,MUX5);
	// set(ADMUX,MUX2);
	// set(ADMUX,MUX1);
	// clear(ADMUX,MUX0);
	// break;
	// case 5: // Set ADC to F7
	// clear(ADMUX,MUX5);
	// set(ADMUX,MUX2);
	// set(ADMUX,MUX1);
	// set(ADMUX,MUX0);
	// break;
	// case 6: // Set ADC to D4
	// set(ADMUX,MUX5);
	// clear(ADMUX,MUX2);
	// clear(ADMUX,MUX1);
	// clear(ADMUX,MUX0);
	// break;
	// case 7: // Set ADC to D6
	// set(ADMUX,MUX5);
	// clear(ADMUX,MUX2);
	// clear(ADMUX,MUX1);
	// set(ADMUX,MUX0);
	// break;
	default:
	break;
}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC);//start next conversion
}