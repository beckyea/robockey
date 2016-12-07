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

enum Bot currBot = OFF1;    // CHANGE THIS VALUE WHEN FLASHING CODE TO DIFFERENT BOTS

// INITIALIZE VALUES
enum Color teamColor = RED;	// team color, from defined enum, either RED or BLUE
char ROBOT_ADDRESS = 0x18;	// mRF address for robot communication
int posX = 500;			    // x position of robot, from -115 to 115
int posY = 500;			    // y position of robot, from -60 to 60
double theta = 0;		    // angle of robot from global frame
double rangeVal = 0;	    // higher number means closer to bot
double puckAngle = 0;	    // angle from the perpendicular 
int defensiveGoalX = 115;  // x position of goal you're defending
int offensiveGoalX = -115;   // x position of goal you're attacking
int minTraversalX = -patrolXRange; // x position of goal you're defending
int maxTraversalX = patrolXRange; // x position of goal you're attacking
int ourScore = 0;		   // score of your team
int otherScore = 0;		   // score of the other team
long time = 0;   		   // total amount of time elapsed (milli-seconds)
enum State gameState = PATROL;
char buffer[10];

void readBuffer();

int main() {
	init_all();
	drive_init();
	m_clockdivide(0);
	//setAmbient();
	clock_init();
	gameState = PATROL;
	while (true) {
		loc_readWii();
		puck_getADCValues();
		switch (gameState) {
			case(NOT_IN_PLAY): 
				stop(); 
				break;
			case (PATROL):
				if (seesPuck()) { gameState = GO_TO_PUCK; }
				else { patrol(); }
				break;
			case (GO_TO_PUCK):
				if (!seesPuck()) { gameState = PATROL; }
				else if (hasPuck()) { gameState = GO_TO_GOAL; }
				else { setDriveToPuck(); }
				break;
			case (GO_TO_GOAL):
				if (!hasPuck() && !seesPuck()) { gameState = PATROL; }
				else if (!hasPuck()) { gameState = GO_TO_PUCK; }
				else { goToGoal(); }
				break;
			default:
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
			if (teamColor == RED) { set(PORTB, 1); m_wait(LED_FLASH_TIME); clear(PORTB, 2); } 
			else if (teamColor == BLUE) { set(PORTB, 2); m_wait(LED_FLASH_TIME); clear(PORTB, 1); }
			break;
		case 0xA1: // Play
			loc_readWii();
			teamColor = loc_getSide();
			if (teamColor == RED) { set(PORTB, 1);  clear(PORTB, 2); } // Left Side
			else { set(PORTB, 2); clear(PORTB, 1); } // Right Side
			init_setGoal();
			gameState = GO_TO_PUCK;
			clock_reset();
			break;
		case 0xA2: // Goal R
			if (teamColor == RED) { ourScore = buffer[1]; otherScore = buffer[2]; celebrate(); }
			else { ourScore = buffer[2]; otherScore = buffer[1]; }
			gameState = NOT_IN_PLAY;
			break;
		case 0xA3: // Goal B
			if (teamColor == BLUE) { ourScore = buffer[2]; otherScore = buffer[1]; celebrate(); }
			else { ourScore = buffer[1]; otherScore = buffer[2]; }
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

// Interrupt to Handle mRF Communication
ISR(INT2_vect) {
	m_rf_read(buffer, PACKET_LENGTH);
	readBuffer();
}