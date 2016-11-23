/**
* Handles all of the robot initialization
* Should only be called once when the robot is turned on.
* Sets inputs, outputs, etc.
*/
#include "initialization.h"
#include "vals.h"

char wiiConnected = 0;

// Initializes all Subsystems
void init_all (void) {
	m_usb_init();
	m_bus_init();
	sei();
	init_mwii();
	init_mrf();
	init_driver();
}

// Initializes the mWii
void init_mwii(void) {
	wiiConnected = m_wii_open();
	while (!wiiConnected) {
		m_red(ON); // pause device until Wii is connected
	}
	m_red(OFF);
}

// Initializes mRF Sesnsor
void init_mrf(void) {
	m_rf_open(CHANNEL, ROBOT_ADDRESS, PACKET_LENGTH);
}

// Initializes Timer 1, pins for Motor Output
void init_driver(void) {
	set(DDRB,6); //Set B6 to output
	set(TCCR1B,WGM13); set(TCCR1B,WGM12); set(TCCR1A,WGM11); set(TCCR1A,WGM10); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,CS12); clear(TCCR1B,CS11); set(TCCR1B,CS10); // set timer prescaler to 1024
	set(TCCR1A,COM1B1); set(TCCR1A,COM1B0); //Set at OCR1B, clear at rollover
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 10000; // Default frequency: 800Hz
	OCR1B = 5000; // ^ duty cycle: 50%
	
}