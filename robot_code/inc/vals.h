// -----------------------------------------------------------------------------
// Header File for Global Variables
// -----------------------------------------------------------------------------

#ifndef vals__
#define vals__

#define CHANNEL 1
#define PACKET_LENGTH 10
#define MOTOR_EN 0

extern char ROBOT_ADDRESS;

extern int posX;
extern int posY;
extern double theta;

enum Color { RED = 1, BLUE = 2, ERROR = 0 };
enum Bot { GOALIE = 0, OFF1 = 1, OFF2 = 2 };

#endif