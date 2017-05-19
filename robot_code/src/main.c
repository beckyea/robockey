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

#define LED_FLASH_TIME 500

enum Bot currBot = OFF2;    // CHANGE THIS VALUE WHEN FLASHING CODE TO DIFFERENT BOTS

// INITIALIZE VALUES
enum Color teamColor = NONE;	// team color, from defined enum, either RED or BLUE
enum Direction offDir = POSITIVE;
char ROBOT_ADDRESS = 0x19;	// mRF address for robot communication
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
enum State gameState;
char buffer[10];
char mrF_Interrupt = 0;

void readBuffer();

int main() {
	init_all();
	drive_init();
	m_clockdivide(0);
	setAmbient();
	clock_init();
	gameState = NOT_IN_PLAY;
	while (true) {
		if (mrF_Interrupt) {
			m_rf_read(buffer, PACKET_LENGTH);
			readBuffer();
			mrF_Interrupt = 0;
		}
		loc_readWii();
		puck_getADCValues();
		normalizePTs();
		switch (gameState) {
			case(NOT_IN_PLAY): 
				stop(); 
				break;
			case (MATCH_START):
				fwd_fast(); m_wait(50); gameState = PATROL;
				break;
			case (PATROL):
				if (seesPuck()) { gameState = GO_TO_PUCK; }
				else { patrol(); }
				break;
			case (GO_TO_PUCK):
				if (!checkInBounds()) { gameState = NO_GO_ZONE; }
				else if (!seesPuck()) { setPatrolDirection(); gameState = PATROL; }
				else if (hasPuck()) { gameState = GO_TO_GOAL; }
				else { setDriveToPuck(); }
				break;
			case (GO_TO_GOAL):
				if (!checkInBounds()) { gameState = NO_GO_ZONE; }
				else if (!hasPuck() && !seesPuck()) { setPatrolDirection(); gameState = PATROL; }
				else if (!hasPuck()) { gameState = GO_TO_PUCK; }
				else { goToGoal(); }
				break;
			case (NO_GO_ZONE):
				if (hasPuck()) { stop(); }
				else if (seesPuck()) { 
					face_puck(); 
					if ((offDir == POSITIVE && theta < 0) || (offDir == NEGATIVE && theta > 0)) { gameState = PATROL; }
				}
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
			if (teamColor == RED) { set(PORTB, 1); clear(PORTB, 2); m_wait(LED_FLASH_TIME); clear(PORTB, 1); } 
			else if (teamColor == BLUE) { set(PORTB, 2); clear(PORTB, 1); m_wait(LED_FLASH_TIME); clear(PORTB, 2); }
			teamColor = NONE;
			break;
		case 0xA1: // Play
			drive_init();
			loc_readWii();
			if (teamColor == NONE) {
				teamColor = loc_getSide();
				if (teamColor == RED) { set(PORTB, 1);  clear(PORTB, 2); } // Left Side
				else { set(PORTB, 2); clear(PORTB, 1); } // Right Side
				init_setSide(0);
			} else {
				if (teamColor == RED) { set(PORTB, 1);  clear(PORTB, 2); } // Left Side
				else { set(PORTB, 2); clear(PORTB, 1); } // Right Side
			}
			gameState = MATCH_START;
			clock_reset();
			break;
		case 0xA2: // Goal R
			if (teamColor == RED) { ourScore = buffer[1]; otherScore = buffer[2]; celebrate(); }
			else { ourScore = buffer[2]; otherScore = buffer[1]; }
			gameState = NOT_IN_PLAY; stop();
			break;
		case 0xA3: // Goal B
			if (teamColor == BLUE) { ourScore = buffer[2]; otherScore = buffer[1]; celebrate(); }
			else { ourScore = buffer[1]; otherScore = buffer[2]; }
			gameState = NOT_IN_PLAY; stop();
			break;
		case 0xA4: // Pause
			gameState = NOT_IN_PLAY; stop();
			break;
		case 0xA5: // Halftime
			gameState = NOT_IN_PLAY; stop();
			init_setSide(1);
			break;
		case 0xA6: // Game Over
			clear(PORTC, 6); clear(PORTC, 7);
			gameState = NOT_IN_PLAY; stop();
			break;
	}
}

// Interrupt to Handle mRF Communication
ISR(INT2_vect) {
	mrF_Interrupt = 1;
}