// -----------------------------------------------------------------------------
// Header File for Global Variables
// -----------------------------------------------------------------------------

#ifndef vals__
#define vals__

// Values for General Robot Definition
#define MOTOR_EN 3
extern enum Color teamColor;
extern enum Bot currBot;
extern enum State gameState;

// Field Values - Change these for Tuning to the Field
#define goalRange 27    	// length of the goal in the y direction from the center in cm
extern int defensiveGoalX;

// Values for mRF Sensor
#define CHANNEL 1
#define PACKET_LENGTH 10
extern char ROBOT_ADDRESS;

// This Robot's Position Global Variables
extern int posX;
extern int posY;
extern double theta;

// Values for Puck Finding
extern double rangeVal;
extern double puckAngle;

// Enumerations
enum Color { RED = 1, BLUE = 2 };
enum Bot { GOALIE, OFF1, OFF2 };
enum State { NOT_IN_PLAY, SEES_LEFT, SEES_RIGHT, GO_CENTER };

#endif