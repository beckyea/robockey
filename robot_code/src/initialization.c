/**
* Handles all of the robot initialization
* Should only be called once when the robot is turned on.
* Sets inputs, outputs, etc.
*/
#include "initialization.h"
#include "m_general.h"
#include "m_usb.h"
#include "m_wii.h"
#include "m_bus.h"
#include "m_rf.h"
#include "puckfind.h"
#include "vals.h"

// Initializes all Subsystems
void init_all (enum Bot bot) {
	m_usb_init();
	m_bus_init();
	sei();
	set(DDRC, 6); // Configure C6 for output -- Positioning LED, RED
	set(DDRC, 7); // Configure C7 for output -- Positioning LED, BLUE
	init_setRobot(bot);
	//init_mwii();
	//init_mrf();
	init_driver();
	init_adc();
}

// Initializes the mWii
void init_mwii(void) {
	int wiiConnected;
	wiiConnected = m_wii_open();
	while (!wiiConnected) {
		m_green(ON); // pause device until Wii is connected
	}
	m_green(OFF);
}

// Initializes mRF Sesnsor
void init_mrf(void) {
	m_rf_open(CHANNEL, ROBOT_ADDRESS, PACKET_LENGTH);
}

// Initializes for Puck Finding
void init_adc(void) {
	m_disableJTAG(); 
	clear(ADMUX, REFS0); // Make Vcc the reference voltage
	set(ADMUX, REFS1);
	clear(ADCSRA, ADPS2); set(ADCSRA, ADPS1); set(ADCSRA, ADPS0); //Set Clock prescaler to /8 => ADC runs at 125kHz
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

	set(ADCSRA,ADEN); //Enable ADC
	set(ADCSRA,ADSC); // Start conversion
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

void init_setRobot(enum Bot bot) {
	switch(bot) {
		case GOALIE:
			ROBOT_ADDRESS = (char)0x18;
			break;
		case OFF1:
			ROBOT_ADDRESS = (char)0x19;
			break;
		case OFF2:
			ROBOT_ADDRESS = (char)0x1A;
			break;
	}
}

void init_setGoal() {
	switch(teamColor) {
		case(RED):  offensiveGoalX = 115;  defensiveGoalX = -115; break;
		case(BLUE): offensiveGoalX = -115; defensiveGoalX = 115; break;
	}

}