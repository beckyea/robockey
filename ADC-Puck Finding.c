
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

volatile int ADC_flag = 0;
int ADC_Channels[8] = {0,0,0,0,0,0,0,0}; //Array to hold phototransistor ADC values
int ADC_Check=0; //Int to iterate through phototransistor channels

//Prototyping subroutines 
void init(void);
void ADC_init(void);

int main(void) {
	init();
	ADC_init();
	set(ADCSRA,ADEN); //Enable ADC
	set(ADCSRA,ADSC); // Start conversion
	
	while(1) {
		if (ADC_flag!=0){  //If ADCs are being read
			m_red(TOGGLE);
			clear(ADCSRA,ADEN); // Disable ADC
			m_usb_tx_string("\nF0:");
			m_usb_tx_int(ADC_Channel[0]);
			m_usb_tx_string("\tF1: ");
			m_usb_tx_int(ADC_Channel[1]);

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
	set(ADMUX, REFS0); // Make Vcc the reference voltage
	clear(ADMUX, REFS1);
	set(ADCSRA, ADPS2); //Set Clock prescaler to /128 => ADC runs at 125kHz
	set(ADCSRA, ADPS1); 
	set(ADCSRA, ADPS20); 
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

ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN);
	ADC_Channels[ADC_Check]=ADC;
	if(ADC_Check==7){   //If all channels read
		ADC_Check == 0; 
		ADC_Flag == 1; 
	}
	else ADC_Check ++;
	if(ADC_Check == 0){ // Set ADC to F0
		clear(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}
	else if(ADC_Check == 1){ // Set ADC to F1
		clear(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
	}
	else if(ADC_Check == 2){ // Set ADC to F4
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}
	else if(ADC_Check == 3){ // Set ADC to F5
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
	}
	else if(ADC_Check == 4){ // Set ADC to F6
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}
	else if(ADC_Check == 5){ // Set ADC to F7
		clear(ADMUX,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		set(ADMUX,MUX0);
	}
	else if(ADC_Check == 6){ // Set ADC to D6
		set(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
	}
	else if(ADC_Check == 7){ // Set ADC to D7
		set(ADMUX,MUX5);
		clear(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
	}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC);//start next conversion
}