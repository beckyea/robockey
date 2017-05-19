// -----------------------------------------------------------------------------
// Header File for Drive Commands
// These commands handle the different driving configurations for the robot.
// Higher levels of abstraction, i.e. go to point, are preferred over lower-level
// code.
// -----------------------------------------------------------------------------

void drive_init(void);        // initializes the motors
void stop(void);              // stops the robot
int goToPoint(int x, int y);  // drives the robot to given point
void drive_test(void);        // test code to validate motor setup and drivetrain

void fwd_fast(void);   // moves robot forward quicky
void fwd_slow(void);   // moves robot forward slowly
void rev_fast(void);   // moves robot backwards quickly
void rev_slow(void);   // moves robot backwards slowly
void right(void);      // turns robot right - ok speed for holding puck
void left(void);       // turns robot left - ok speed for holding puck
void right_slow(void); // turns robot right slowly
void left_slow(void);  // turns robot left slowly
void right_ip(void);   // turns robot right in place (will lose puck!)
void left_ip(void);    // turns robot left in place (will lose puck!)


void turnInPlaceByTheta(double radians); // turns in place by specified radians
void turnInPlaceToTheta(double endTheta); // turns in place to reach a orientation in radians

void patrol(void); 				// sends robot on patrol path
void goToPuck(void); 			// sends robot towards the puck
void goToGoal(void); 			// sends the robot to the goal using only turns ok for puck possession
void celebrate(void);  			// goal scored! spin in circles
void setPatrolDirection(void);  // determines patrol direction