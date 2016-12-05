 /**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
* TO DO - CONVERT THE DRIVE FUNCTIONS TO MACROS
*/

#include "initialization.h"
#include "vals.h"
#include "m_general.h"
#include "drive.h"
#include <stdlib.h>

#define MOTOR_PORT PORTB

#define PATROL_X_BUFFER 50
#define PATROL_Y_HEIGHT 20

#define DRIVE_ALPHA 0.2

#define K_P 1
#define K_I 0
#define K_D 0

double thetaPID(double desiredTheta);
double thetaThreshold = 0.2;
double integral, preverror;
int deltat;

int OCR4A_desired, OCR4B_desired;

void setLeftFwd(void);
void setLeftRev(void);
void setRightFwd(void);
void setRightRev(void);
void setDrive(void);


// Initializes the motors
void drive_init(void) { 
	set(DDRB,MOTOR_EN); // Define output pins for Motor Control
}

void setLeftFwd(void)  { set(PORTC, 7); clear(PORTC, 6); }
void setLeftRev(void)  { clear(PORTC, 7); set(PORTC, 6); }
void setRightFwd(void) { set(PORTB, 5); clear(PORTB, 6); }
void setRightRev(void) { clear(PORTB, 5); set(PORTB, 6); }
void stop(void)        { clear(TCCR4B,CS43); clear(TCCR4B,CS42); clear(TCCR4B,CS41); clear(TCCR4B,CS40); }

// Set Desired PWM Output
void fwd_fast(void) { OCR4A_desired = 255;  OCR4B_desired = 255;  setDrive(); }
void fwd_slow(void) { OCR4A_desired = 150;  OCR4B_desired = 150;  setDrive(); }
void rev_fast(void) { OCR4A_desired = -1;   OCR4B_desired = -1;   setDrive(); }
void rev_slow(void) { OCR4A_desired = -105; OCR4B_desired = -105; setDrive(); }
void right(void)    { OCR4A_desired = 126;  OCR4B_desired = 63;   setDrive(); }
void left(void)     { OCR4A_desired = 63;   OCR4B_desired = 0;    setDrive(); }
void right_ip(void) { OCR4A_desired = 126;  OCR4B_desired = -129; setDrive(); }
void left_ip(void)  { OCR4A_desired = -129; OCR4B_desired = 126;  setDrive(); }

void setDrive(void) {
	int OCR4A_curr, OCR4B_curr;
	if (check(PINC, 7)) { OCR4A_curr = OCR4A; }
	else { OCR4A_curr = - OCR4A; }
	if (check(PINB, 5)) { OCR4B_curr = OCR4B; }
	else { OCR4B_curr = - OCR4B; }
	OCR4A_curr = (int) (OCR4A_curr * DRIVE_ALPHA + (1 - DRIVE_ALPHA) * OCR4A_desired);
	OCR4B_curr = (int) (OCR4B_curr * DRIVE_ALPHA + (1 - DRIVE_ALPHA) * OCR4B_desired);
	if (OCR4A_curr < 0) { OCR4A = -OCR4A_curr; setLeftRev(); }
	else if (OCR4A_curr > 0) { OCR4A = OCR4A_curr; setLeftFwd(); }
	else { OCR4A = 0; 
		if (OCR4A_desired < 0) { setLeftRev(); } 
		else { setLeftFwd(); }
	}
	if (OCR4B_curr < 0) { OCR4B = -OCR4B_curr; setRightRev(); }
	else if (OCR4B_curr > 0) { OCR4B = OCR4B_curr; setRightFwd(); }
	else { OCR4B = 0; 
		if (OCR4B_desired < 0) { setLeftRev(); } 
		else { setLeftFwd(); }
	}
}

// Test Code to test Motor Controller
void drive_test(void) {
	fwd_fast();
	m_wait(1000);
	rev_slow();
	m_wait(1000);
	right();
	m_wait(1000);
	left();
	m_wait(1000);
	stop();
}

// Returns Output of PID comparison on Omega
double thetaPID(double desiredTheta) {
	double error, output;
	error = desiredTheta - theta;
	integral += error * deltat;
	output = K_P * error + K_I * integral + K_D * (error - preverror)/deltat;
	preverror = error;
	return output;
}


// Turns in place by a certain number of radians
void turnInPlaceByTheta(double radians) {
	if (radians < 0) { radians += 2 * 3.1416; }
	double endTheta = theta + radians;
	if (endTheta > 2 * 3.1416) { endTheta -= 2 * 3.1416; }
	if (radians < 3.1416) {
		while (abs(endTheta - theta) > thetaThreshold) { right_ip(); }
	}  else {
		while (abs(endTheta - theta) > thetaThreshold) { left_ip(); }
	}
}

// Turns in place to reach a given orientation in radians
void turnInPlaceToTheta(double endTheta) {
	if (endTheta < 0) { endTheta += 2 * 3.1416; }
	if (endTheta < 3.1416) {
		while (abs(endTheta - theta) > thetaThreshold) { right_ip(); }
	}  else {
		while (abs(endTheta - theta) > thetaThreshold) { left_ip(); }
	}
}

void celebrate(void) {
	right_ip();
	m_wait(1000);
	stop();
}


void patrol(void) {
	if (currBot == OFF1) {
		if (velX > 0 && posX < 115 - PATROL_X_BUFFER) { 
			goToPoint(115 - PATROL_X_BUFFER, PATROL_Y_HEIGHT); 
		} else if (velX <= 0  && posX < -115 + PATROL_X_BUFFER) {
			goToPoint(PATROL_X_BUFFER - 115, PATROL_Y_HEIGHT);
		}
	} else if (currBot == OFF2) {
		if (velX > 0 && posX < 115 - PATROL_X_BUFFER) { 
			goToPoint(115 - PATROL_X_BUFFER, -PATROL_Y_HEIGHT); 
		} else if (velX <= 0  && posX < -115 + PATROL_X_BUFFER) {
			goToPoint(PATROL_X_BUFFER - 115, -PATROL_Y_HEIGHT);
		}
	}
}

void goToPuck(void) {
	if (puckAngle < .08 || puckAngle > 6.2) { fwd_fast(); }
	else if (puckAngle < 3.1416) { right_ip(); }
	else { left_ip(); }
}

void goToGoal(void) {
	goToPoint(offensiveGoalX, 0); // TODO: Change this to follow a go-to-goal with puck (maintains minimum turning radius)
}

// Go to point (x,y)
int goToPoint(int x, int y) {
	double thetaToPos, theta_temp;
	thetaToPos = (double) (atan2(posY - y, posX - x)) - 3.1416/2;
	if (thetaToPos < - 3.1416) { thetaToPos = thetaToPos + 3.1416*2; }
	theta_temp = theta;
	if (abs(posX - x) < 10 && abs(posY - y) < 10) {
		stop();
		return 1;
	} else if ((theta_temp - thetaToPos) > .5) {
		right();
	} else if ((thetaToPos - theta_temp) > .5) {
		left();
	} else { fwd_fast(); }
	// m_usb_tx_int((int) (posX));
	// m_usb_tx_string("\t");
	// m_usb_tx_int((int) (posY));
	// m_usb_tx_string("\t");
	// m_usb_tx_int((int) (theta_temp*1000));
	// m_usb_tx_string("\t");
	// m_usb_tx_int((int) (thetaToPos*1000));
	// m_usb_tx_string("\t");
	// m_usb_tx_int((int) ((theta_temp - thetaToPos)*100));
	// m_usb_tx_string("\n");
	return 0;
}

