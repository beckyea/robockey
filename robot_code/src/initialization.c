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
void init_all (void) {
	m_usb_init();
	m_bus_init();
	sei();
	set(DDRB, 1); // Configure B1 for output -- Positioning LED, RED
	set(DDRB, 2); // Configure B2 for output -- Positioning LED, BLUE
	set(DDRB, 0); // Configure B0 for output -- Yellow LED
	init_setRobot();
	init_mwii();
	init_mrf();
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
	set(ADCSRA, ADPS2); set(ADCSRA, ADPS1); set(ADCSRA, ADPS0); //Set Clock prescaler to /128 => ADC runs at 125kHz
	//Disable digital on ADC Pins
	set(DIDR0,ADC0D); // F0
	set(DIDR2,ADC10D); // D7
	set(DIDR0,ADC4D); // F4
	set(DIDR0,ADC5D); // F5
	set(DIDR0,ADC6D); // F6
	set(DIDR0,ADC7D); // F7
	set(DIDR2,ADC8D); // D4
	// set(DIDR2,ADC9D); // D6
	set(DIDR2,ADC11D); // B4
	set(ADCSRA,ADIE); //Enable ADC Interrupt Flag
	clear(ADCSRA,ADATE); // Disable free-running Mode

	set(ADCSRA,ADEN); //Enable ADC
	set(ADCSRA,ADSC); // Start conversion
}

// // Initializes Timer 4, pins for Motor Output
void init_driver(void) {
	set(DDRD,3); //D3 --> A0 (MOTOR ENABLE)
	set(DDRC,6); //C6 --> D7 (INA1)
	set(DDRB,6); //B6 --> D4 (INA2)
	set(DDRC,7); //C7 --> D8 (INB1)
	set(DDRB,5); //B5 --> D9 (INB2)

	set(PORTC,6); //C6 --> D7 (INA1)
	set(PORTB,6); //B6 --> D4 (INA2)
	set(PORTC,7); //C7 --> D8 (INB1)
	set(PORTB,5); //B5 --> D9 (INB2)

	set(DDRD,MOTOR_EN);
	
	// Initialize Timer 4
	set(TCCR4B,CS43); clear(TCCR4B,CS42); clear(TCCR4B,CS41); clear(TCCR4B,CS40);// set timer prescaler to /64
	clear(TCCR4D,WGM41); clear(TCCR4D,WGM40); // Count UP to OCR4C
	set(TCCR4A,PWM4A); set(TCCR4A,COM4A1); clear(TCCR4A,COM4A0); //PWM, set at rollover, clear at OCR4A
	set(TCCR4A,PWM4B); set(TCCR4A,COM4B1); clear(TCCR4A,COM4B0); //PWM, set at rollover, clear at OCR4B
	
	OCR4C = 255;
	OCR4A = 255;
	OCR4B = 255;
}

void init_setRobot(void) {
	switch (currBot) {
		case GOALIE:
			ROBOT_ADDRESS = (char) 0x1A;
			break;
		case OFF1:
			ROBOT_ADDRESS = (char) 0x18;
			break;
		case OFF2:
			ROBOT_ADDRESS = (char) 0x19;
			break;
	}
}

// Input 0 if on side matching color, 1 for second half
void init_setSide(int i) {
	if ((teamColor == RED && i == 0) || (teamColor == BLUE && i == 1)) {
		offDir = NEGATIVE;
		offensiveGoalX = -125; defensiveGoalX = 125;  maxTraversalX = 115 - goalBoxXLength; minTraversalX = -125;
	} else {
		offDir = POSITIVE;
		offensiveGoalX = 125;  defensiveGoalX = -125; minTraversalX = - 115 + goalBoxXLength; maxTraversalX = 125;
	}
}