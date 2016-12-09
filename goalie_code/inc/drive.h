// -----------------------------------------------------------------------------
// Header File for Drive Commands
// These commands handle the different driving configurations for the robot.
// Higher levels of abstraction, i.e. go to point, are preferred over lower-level
// code.
// -----------------------------------------------------------------------------

void drive_init(void);
// FUNCTION: Initializes the motors
void stop(void);
void right(void);
void left(void);
void patrol(void);