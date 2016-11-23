/**
* General Controller Class
* Handles Play Commands, Driving, State of the Robot
*/
#include "m_general.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_bus.h"
#include "initialization.h"
#include "localization.h"
#include "vals.h"
#include "drive.h"

#define LED_FLASH_TIME 1000

char scoreA = 0;
char scoreB = 0;
char inPlay = 1;
enum color currTeam;

char buffer[10];

void readBuffer();

// Main Function
int main() {
	init_all();
	drive_init();
	set(DDRE, 6); // Configure E6 for output -- Positioning LED, RED
	set(DDRC, 7); // Configure C7 for output -- Positioning LED, BLUE
	m_clockdivide(4);
	while (inPlay) {
		loc_readWii();
		goToPoint(0, 115);
	}
	return 0;
}

// Reads the Buffer and Performs Appropriate Action
void readBuffer() {
	switch ((unsigned char) buffer[0]) {
		case 0xA0 : // Communication Test
			loc_readWii();
			if (loc_getSide() == 1) { // Left Side - Display Red LED
				set(PORTE, 6);
				m_wait(LED_FLASH_TIME);
				clear(PORTE, 6);
			} else if (loc_getSide() == 2) { // Right Side - Display Blue LED
				set(PORTC, 7);
				m_wait(LED_FLASH_TIME);
				clear(PORTC, 7);
			} else {} // TODO - HANDLE THIS BECAUSE SOMETHING IS WRONG
			break;
		case 0xA1: // Play
			loc_readWii();
			currTeam = loc_getSide();
			if (currTeam == RED) { set(PORTE, 6); } // Left Side
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
ISR(INT2_vect) {
	m_rf_read(buffer, PACKET_LENGTH);
	readBuffer();
}