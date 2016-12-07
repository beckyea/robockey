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

#define LED_FLASH_TIME 50

enum Bot currBot = GOALIE; 

// INITIALIZE VALUES
enum Color teamColor = RED;	// team color, from defined enum, either RED or BLUE
char ROBOT_ADDRESS = (char) 0x1A;	// mRF address for robot communication
int posX = 500;			    // x position of robot, from -115 to 115
int posY = 500;			    // y position of robot, from -60 to 60
double theta = 0;		    // angle of robot from global frame
double rangeVal = 0;	    // higher number means closer to bot
double puckAngle = 0;	    // angle from the perpendicular 
int defensiveGoalX = -115;  // x position of goal you're defending
enum State gameState = NOT_IN_PLAY;
char buffer[10];

void readBuffer();

// Main Function
int main() {
	init_all();
	drive_init();
	m_clockdivide(0);
	while (true) {
		switch (gameState) {
			case(NOT_IN_PLAY): 
				stop(); 
				break;
			case (SEES_LEFT):
				loc_readWii();
				puck_getADCValues();
				puck_drive();
				left();
				break;
			case (SEES_RIGHT):
				loc_readWii();
				puck_getADCValues();
				puck_drive();
				right(); 
				break;
			case (GO_CENTER):
				loc_readWii();
				puck_drive();
				goToCenter(); 
				break;
		}
	}
	return 0;
}

// Reads the Buffer and Performs Appropriate Action
void readBuffer() {
	switch ((unsigned char) buffer[0]) {
		case 0xA0 : // Communication Test
			loc_readWii();
			teamColor = loc_getSide();
			init_setGoal();
			if (teamColor == RED) { set(PORTB, 1); m_wait(LED_FLASH_TIME); clear(PORTB, 2); } 
			else if (teamColor == BLUE) { set(PORTB, 2); m_wait(LED_FLASH_TIME); clear(PORTB, 1); }
			break;
		case 0xA1: // Play
			loc_readWii();
			teamColor = loc_getSide();
			if (teamColor == RED) { set(PORTB, 1);  clear(PORTB, 2); } // Left Side
			else { set(PORTB, 2); clear(PORTB, 1); } // Right Side
			init_setGoal();
			gameState = GO_CENTER;
			break;
		case 0xA2: // Goal R
			gameState = NOT_IN_PLAY;
			break;
		case 0xA3: // Goal B
			gameState = NOT_IN_PLAY;
			break;
		case 0xA4: // Pause
			gameState = NOT_IN_PLAY;
			break;
		case 0xA5: // Halftime
			gameState = NOT_IN_PLAY;
			break;
		case 0xA6: // Game Over
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