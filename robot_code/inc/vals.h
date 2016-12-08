// -----------------------------------------------------------------------------
// Header File for Global Variables
// -----------------------------------------------------------------------------

#ifndef vals__
#define vals__

// Values for General Robot Definition
#define MOTOR_EN 3
extern enum Color teamColor;
extern enum Bot currBot;
extern enum Direction offDir;

// Field Values - Change these for Tuning to the Field
#define goalRange 27    	// length of the goal in the y direction from the center in cm
#define patrolXRange 87 	// magnitude of x value from center for patrol
#define patrolYVal 20		// magnitude of y value from central axis for patrol
#define goalBoxXLength 30	// minimum distance for bot to travel from own goal
extern int offensiveGoalX;
extern int defensiveGoalX;
extern int minTraversalX;
extern int maxTraversalX;

// Values for mRF Sensor
#define CHANNEL 1
#define PACKET_LENGTH 10
extern char ROBOT_ADDRESS;

// This Robot's Position Global Variables
int posX;
int posY;
extern double theta;
extern double velX;
extern double velY;
extern double omega;
extern int deltat;

// Values for Puck Finding
extern double rangeVal;
extern double puckAngle;

// Values Describing the Game
extern int ourScore;
extern int otherScore;
extern long time;

// Enumerations
enum Color { NONE = 0, RED = 1, BLUE = 2 };
enum Direction { POSITIVE, NEGATIVE };
enum Bot { GOALIE, OFF1, OFF2 };
enum State { NOT_IN_PLAY, PATROL, GO_TO_PUCK, GO_TO_GOAL, HALF_PATROL, GO_TO_BOX_CORNER };

#endif