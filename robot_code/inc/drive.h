// -----------------------------------------------------------------------------pa=z
// Header File for Drive Commands
// These commands handle the different driving configurations for the robot.
// Higher levels of abstraction, i.e. go to point, are preferred over lower-level
// code.
// -----------------------------------------------------------------------------

void drive_init(void);
// FUNCTION: Initializes the motors

void fwd(void);
// FUNCTION: Drives the robot forward

void rev(void);
// FUNCTION: Drives the robot backwards

void left(void);
// FUNCTION: Turns the robot left (in place)

void right(void);
// FUNCTION: Turns the robot right (in place)

void stop(void);
// FUNCTION: Stops the robot

int goToPoint(int x, int y);
// FUNCTION: Drives the Robot to the Desirem8d Point
// INPUTS: 2 int values, designating x and y positions to be reached

void test(void);

// Turns in place by a certain number of radians
void turnInPlaceByTheta(double radians);

// Turns in place to reach a given orientation in radians
void turnInPlaceToTheta(double endTheta);

void patrol(void);

void goToGoal(void);

void celebrate(void);