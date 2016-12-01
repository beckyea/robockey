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
#include "puckfind.h"

#define LED_FLASH_TIME 50

// INITIALIZE VALUES
enum Color teamColor = RED;	// team color, from defined enum, either RED or BLUE
char ROBOT_ADDRESS = 0x00;	// mRF address for robot communication
int posX = 500;			// x position of robot, from -115 to 115
int posY = 500;			// y position of robot, from -60 to 60
double theta = 0;		// angle of robot from global frame
char hasPuck = 0; 		// 0 if no puck, 1 if has puck
char seesPuck = 0;		// 0 if does not see puck, 1 if sees puck
double rangeVal = 0;	// higher number means closer to bot
double puckAngle = 0;	// angle from the perpendicular 
int defensiveGoalX = -115; // x position of goal you're defending
int offensiveGoalX = 115; // x position of goal you're attacking
int ourScore = 0;		// score of your team
int otherScore = 0;		// score of the other team
char inPlay = 0;  // 0 if initializing with Play Command, 0 for Testing

char buffer[10];

void readBuffer();

// Main Function
int main() {
	init_all(OFF1);
	drive_init();
	m_clockdivide(4);
	setAmbient();
	while (true) {
		if (inPlay) {
			loc_readWii();
			puck_getADCValues();
			goToPoint(offensiveGoalX, 0);
		} else {
			stop();
		}
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
			teamColor = loc_getSide();
			if (teamColor == RED) { set(PORTC, 6);  } // Left Side
			else if (teamColor == BLUE) { set(PORTC, 7); } // Right Side
			else { set(PORTC, 7); }
			init_setGoal();
			fwd(); m_wait(500); rev(); m_wait(500); stop();
			inPlay = 1;
			break;
		case 0xA2: // Goal R
			if (teamColor == RED) {
				ourScore = buffer[1]; otherScore = buffer[2];
				celebrate();
			} else { ourScore = buffer[2]; otherScore = buffer[1]; }
			inPlay = 0;
			break;
		case 0xA3: // Goal B
			if (teamColor == BLUE) {
				ourScore = buffer[2]; otherScore = buffer[1];
				celebrate();
			} else { ourScore = buffer[1]; otherScore = buffer[2]; }
			inPlay = 0;
			break;
		case 0xA4: // Pause
			inPlay = 0;
			break;
		case 0xA5: // Halftime
			inPlay = 0;
			if (teamColor == RED) { clear (PORTC, 6); set(PORTC, 7); teamColor = BLUE; }
			else { clear (PORTC, 6); set(PORTC, 7); teamColor = RED; }
			break;
		case 0xA6: // Game Over
			inPlay = 0;
			break;
	}
}

ISR(TIMER1_COMPA_vect){    //PWM signal goes low
	clear(PORTB,MOTOR_EN);
	clear(PORTB,5);
	clear(PORTB,6);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	set(PORTB,MOTOR_EN);
	set(PORTB,5);
	set(PORTB,6);
}

// Interrupt to Handle mRF Communication
ISR(INT2_vect) {
	m_usb_tx_string("a"); m_usb_tx_push();
	m_rf_read(buffer, PACKET_LENGTH);
	readBuffer();
}