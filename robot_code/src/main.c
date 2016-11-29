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

#define LED_FLASH_TIME 50

char scoreA = 0;
char scoreB = 0;
char inPlay = 0;
enum Color currTeam;

char buffer[10];
int posX = 0;
int posY = 0;
double theta = 0.0;

void readBuffer();

// Main Function
int main() {
	init_all(OFF1);
	drive_init();
	set(DDRC, 6); // Configure C6 for output -- Positioning LED, RED
	set(DDRC, 7); // Configure C7 for output -- Positioning LED, BLUE
	m_clockdivide(4);
	int foundGoal = 0;
	while (inPlay && !foundGoal) {
		loc_readWii();
<<<<<<< Updated upstream
		foundGoal = goToPoint(106, -17);
	}
	while (!inPlay) {
		stop();
=======
		//goToPoint(0, 115);
		m_wait(100);
>>>>>>> Stashed changes
	}
	return 0;
}

// Reads the Buffer and Performs Appropriate Action
void readBuffer() {
	switch ((unsigned char) buffer[0]) {
		case 0xA0 : // Communication Test
			loc_readWii();
			if (loc_getSide() == 1) { // Left Side - Display Red LED
				set(PORTC, 6);
				m_wait(LED_FLASH_TIME);
				clear(PORTC, 6);
			} else if (loc_getSide() == 2) { // Right Side - Display Blue LED
				set(PORTC, 7);
				m_wait(LED_FLASH_TIME);
				clear(PORTC, 7);
			} else {} // TODO - HANDLE THIS BECAUSE SOMETHING IS WRONG
			break;
		case 0xA1: // Play
			loc_readWii();
			currTeam = loc_getSide();
			if (currTeam == RED) { set(PORTC, 6); } // Left Side
			else if (currTeam == BLUE) { set(PORTC, 7); } // Right Side
			else { set(PORTC, 7); }
			fwd(); m_wait(500); rev(); m_wait(500); stop();
			inPlay = 1;
			break;
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
ISR(TIMER1_COMPA_vect){    //PWM signal goes low
	clear(PORTB,MOTOR_EN);
	//clear(PORTB,7);
	clear(PORTB,5);
	clear(PORTB,6);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	set(PORTB,MOTOR_EN);
	//set(PORTB,7);
	set(PORTB,5);
	set(PORTB,6);
}

// Interrupt to Handle mRF Communication
ISR(INT2_vect) {
	m_usb_tx_string("a"); m_usb_tx_push();
	m_rf_read(buffer, PACKET_LENGTH);
	readBuffer();
}