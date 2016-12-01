// -----------------------------------------------------------------------------
// Header File for Global Variables
// -----------------------------------------------------------------------------

#ifndef vals__
#define vals__

// Values for General Robot Definition
#define MOTOR_EN 0
extern enum Color teamColor;

// Field Values - Change these for Tuning to the Field
#define goalRange 27    	// length of the goal in the y direction from the center in cm
#define patrolXRange 87 	// magnitude of x value from center for patrol
#define patrolYVal 20		// magnitude of y value from central axis for patrol
#define goalBoxXLength 30	// minimum distance for bot to travel from own goal

// Values for mRF Sensor
#define CHANNEL 1
#define PACKET_LENGTH 10
extern char ROBOT_ADDRESS;

// This Robot's Position Global Variables
extern int posX;
extern int posY;
extern double theta;
extern char hasPuck;
extern char seesPuck;

// Values for Puck Finding
extern double rangeVal;
extern double puckAngle;

// Values Describing the Field
extern int defensiveGoalX;
extern int offensiveGoalX;

// Values Describing the Game
extern char inPlay;
extern int ourScore;
extern int otherScore;

// Enumerations
enum Color { RED = 1, BLUE = 2 };
enum Bot { GOALIE, OFF1, OFF2 };
enum State { PATROL = 0, GO_TO_PUCK = 1, GO_TO_GOAL = 2, HALF_PATROL = 3, GO_TO_BOX_CORNER = 4 };

// Methods
void vals_reset(void);

#endif