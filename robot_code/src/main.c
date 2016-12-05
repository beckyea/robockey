/**
* General Controller Class
* Handles Play Commands, Driving, State of the Robot
*/
#include "m_general.h"
#include "avr/interrupt.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_bus.h"
#include "initialization.h"
#include "localization.h"
#include "vals.h"
#include "drive.h"
#include "puckfind.h"
#include "clock.h"

#define LED_FLASH_TIME 50

// INITIALIZE VALUES
enum Color teamColor = RED;	// team color, from defined enum, either RED or BLUE
enum Bot currBot = OFF1;
char ROBOT_ADDRESS = 0x18;	// mRF address for robot communication
int posX = 500;			// x position of robot, from -115 to 115
int posY = 500;			// y position of robot, from -60 to 60
double theta = 0;		// angle of robot from global frame
double rangeVal = 0;	// higher number means closer to bot
double puckAngle = 0;	// angle from the perpendicular 
int defensiveGoalX = -115; // x position of goal you're defending
int offensiveGoalX = 115; // x position of goal you're attacking
int ourScore = 0;		// score of your team
int otherScore = 0;		// score of the other team
long time = 0;   		// total amount of time elapsed (milli-seconds)
enum State gameState = NOT_IN_PLAY;
char buffer[10];

void readBuffer();

// Main Function
int main() {
	init_all();
	drive_init();
	m_clockdivide(0);
	setAmbient();
	clock_init();
	while (true) {
		switch (gameState) {
			case(NOT_IN_PLAY): 
				stop(); 
				break;
			case (PATROL):
				loc_readWii();
				puck_getADCValues();
				patrol();
				break;
			case (GO_TO_PUCK):
				loc_readWii();
				puck_getADCValues();
				goToPuck(); 
				break;
			case (GO_TO_GOAL):
				loc_readWii();
				goToPoint(offensiveGoalX, 0); 
				break;
			case (HALF_PATROL):
				loc_readWii();
				puck_getADCValues();
				patrol();
				break;
			case (GO_TO_BOX_CORNER):
				loc_readWii();
				puck_getADCValues();
				// TO DO - SET POINT TO GO TO
				break;
		}
	}
	return 0;
}

// Reads the Buffer and Performs Appropriate Action
void readBuffer() {
	switch ((unsigned char) buffer[0]) {
		case 0xA0 : // Communication Test
			m_usb_tx_string("A0");
			loc_readWii();
			if (loc_getSide() == 1) { // Left Side - Display Red LED
				set(PORTC, 6);
				m_wait(LED_FLASH_TIME);
				clear(PORTC, 6);
			} else if (loc_getSide() == 2) { // Right Side - Display Blue LED
				set(PORTC, 7);
				m_wait(LED_FLASH_TIME);
				clear(PORTC, 7);
			}
			break;
		case 0xA1: // Play
			m_usb_tx_string("A1");
			loc_readWii();
			teamColor = loc_getSide();
			if (teamColor == RED) { set(PORTC, 6);  clear(PORTC, 7); } // Left Side
			else { set(PORTC, 7); clear(PORTC, 6); } // Right Side
			init_setGoal();
			fwd_slow(); m_wait(500); rev_slow(); m_wait(500); stop();
			gameState = GO_TO_PUCK;
			clock_reset();
			break;
		case 0xA2: // Goal R
			m_usb_tx_string("A2");
			if (teamColor == RED) {
				ourScore = buffer[1]; otherScore = buffer[2];
				celebrate();
			} else { ourScore = buffer[2]; otherScore = buffer[1]; }
			gameState = NOT_IN_PLAY;
			break;
		case 0xA3: // Goal B
			m_usb_tx_string("A3");
			if (teamColor == BLUE) {
				ourScore = buffer[2]; otherScore = buffer[1];
				celebrate();
			} else { ourScore = buffer[1]; otherScore = buffer[2]; }
			gameState = NOT_IN_PLAY;
			break;
		case 0xA4: // Pause
			m_usb_tx_string("A4");
			gameState = NOT_IN_PLAY;
			break;
		case 0xA5: // Halftime
			m_usb_tx_string("A5");
			m_red(ON);
			if (teamColor == RED) { clear (PORTC, 6); m_wait(100); set(PORTC, 6); teamColor = BLUE; }
			else { clear(PORTC, 7); m_wait(100);  set(PORTC, 7); teamColor = RED; }
			gameState = NOT_IN_PLAY;
			break;
		case 0xA6: // Game Over
			m_usb_tx_string("A6");
			clear(PORTC, 6); clear(PORTC, 7);
			gameState = NOT_IN_PLAY;
			break;
	}
}

ISR(TIMER4_COMPA_vect){    //PWM signal goes low
	clear(PORTB,MOTOR_EN);
	clear(PORTB,5);
	clear(PORTB,6);
}

ISR(TIMER4_COMPB_vect){   //PWM signal goes high
	set(PORTB,MOTOR_EN);
	set(PORTB,5);
	set(PORTB,6);
}

// Interrupt to Handle mRF Communication
ISR(INT2_vect) {
	m_rf_read(buffer, PACKET_LENGTH);
	readBuffer();
}