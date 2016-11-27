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
	// // Initialize Timer 4
	// set(TCCR4B, CS43); clear(TCCR4B, CS42); clear(TCCR4B, CS41); clear(TCCR4B, CS40); // clockspeed /64
	// clear(TCCR4D, WGM41); clear(TCCR4D, WGM40); // up to OCR4C
	// OCR4A = 255;
	// set(TIMSK4, TOIE4);
}

// Initializes mRF Sesnsor
void init_mrf(void) {
	m_rf_open(CHANNEL, ROBOT_ADDRESS, PACKET_LENGTH);
}

// Initializes Timer 1, pins for Motor Output
void init_driver(void) {
	set(DDRB,MOTOR_EN); //B0 --> A0
	set(DDRB,1); //B1 --> D7
	set(DDRB,2); //B2 --> D8
	set(DDRB,3); //B3 --> D9
	set(DDRB,4); //B4 --> D4
	set(DDRB,5); //B5 --> D5
	set(DDRB,6); //Set B6 to output
	//Timer 1 config
	set(TCCR1B,WGM13); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,WGM12); // ^
	set(TCCR1A,WGM11); // ^
	set(TCCR1A,WGM10); // ^
	
	clear(TCCR1B,CS12); // set timer prescaler to /64
	clear(TCCR1B,CS11);
	set(TCCR1B,CS10);
	
	set(TCCR1A,COM1B1); //Set at OCR1B, clear at rollover
	set(TCCR1A,COM1B0); // ^
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 10000; // Default frequency: 800Hz
	OCR1B = 8400; // ^ duty cycle: 50%
	
}