// -----------------------------------------------------------------------------
// Header File for Global Variables
// -----------------------------------------------------------------------------

#ifndef vals__
#define vals__


#define ROBOT_ADDRESS 0x48
#define CHANNEL 1
#define PACKET_LENGTH 10
#define MOTOR_EN 0

extern int posX;
extern int posY;
extern double theta;

enum color { RED = 1, BLUE = 2, ERROR = 0 };

#endif