// -----------------------------------------------------------------------------pa=z
// Header File for Drive Commands
// These commands handle the different driving configurations for the robot.
// Higher levels of abstraction, i.e. go to point, are preferred over lower-level
// code.
// -----------------------------------------------------------------------------

void drive_init(void);
// FUNCTION: Initializes the motors
void stop(void);

int goToPoint(int x, int y);
// FUNCTION: Drives the Robot to the Desirem8d Point
// INPUTS: 2 int values, designating x and y positions to be reached

void drive_test(void);

void fwd_fast(void);
void fwd_slow(void);
void rev_fast(void);
void rev_slow(void);
void right(void);
void left(void);
void right_slow(void);
void left_slow(void);
void right_ip(void);
void left_ip(void);

// Turns in place by a certain number of radians
void turnInPlaceByTheta(double radians);

// Turns in place to reach a given orientation in radians
void turnInPlaceToTheta(double endTheta);

void patrol(void);

void goToPuck(void);

void goToGoal(void);

void celebrate(void);