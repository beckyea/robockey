/**
* General Controller Class
* Handles Play Commands, Driving, State of the Robot
*/
#include "m_general.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_bus.h"
#include "localization.c"

#define ROBOT_ADDRESS 0x48
#define CHANNEL 1
#define PACKET_LENGTH 10

enum color { RED = 1, BLUE = 2, ERROR = 0 };

char scoreA = 0;
char scoreB = 0;
char inPlay = 0;
color currTeam;

char wiiConnected = 0;

unsigned int buffer[10];
char LEDflashTime = 100;

void init();
void readBuffer();

// Main Function
void main() {
	init();
}

// Initialization Code
void init() {
	m_usb_init();
	m_bus_init();
	sei();
	wiiConnected = m_wii_open(void);
	while (!wiiConnected) {
		m_red(ON); // pause device until Wii is connected
	}
	m_red(OFF);
	m_rf_open(CHANNEL, ROBOT_ADDRESS, PACKET_LENGTH);
	set(DDRC, 6); // Configure C6 for output -- Positioning LED, RED
	set(DDRC, 7); // Configure C7 for output -- Positioning LED, BLUE
}

// Reads the Buffer and Performs Appropriate Action
void readBuffer() {
	switch buffer[0] {
		case 0xA0 : // Communication Test
			loc_readWii();
			if (loc_getSide() == 1) { // Left Side - Display Red LED
				set(PORTC, 6);
				m_wait(LEDflashTime);
				clear(PORTC, 6);
			} else if (loc_getSide() == 2) { // Right Side - Display Blue LED
				set(PORTC, 7);
				m_wait(LEDflashTime);
				clear(PORTC, 7);
			} else {} // TODO - HANDLE THIS BECAUSE SOMETHING IS WRONG
			break;
		case 0xA1: // Play
			loc_readWii();
			currTeam = loc_getSide();
			if (currTeam == RED) { set(PORTC, 6); } // Left Side
			else if (currTeam == BLUE) { set(PORTC, 7); } // Right Side
			else {}
			inPlay = 1;
			break;
			// move in a noticeable way -- TODO: DRIVE COMMAND
		case 0xA2: // Goal R
			scoreA = buffer[1];
			scoreB = buffer[2];
			break;
		case 0xA3: // Goal B
			scoreA = buffer[1];
			scoreB = buffer[2];
			break;
		case 0xA4: // Pause
			inPlay = 0;
			break;
		case 0xA5: // Halftime
			inPlay = 0;
			break;
		case 0xA6: // Game Over
			inPlay = 0;
			break;

	}
}


// Interrupt to Handle mRF Communication
ISR(INT2_VECT) {
	mread(buffer, PACKET_LENGTH);
	readBuffer();
}