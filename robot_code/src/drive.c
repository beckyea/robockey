 /**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
*/

#include "initialization.h"
#include "vals.h"
#include "m_general.h"
#include "m_usb.h"
#include "drive.h"
#include "puckfind.h"
#include <stdlib.h>

#define MOTOR_PORT PORTB

#define DRIVE_ALPHA 0.2
#define LEFT_OVER_RIGHT 2
#define K_P 1
#define K_I 0
#define K_D 0

double thetaPID(double desiredTheta);
double omegaPID(double desiredTheta);
double thetaThreshold = 0.2;
double integral_theta, preverror_theta, integral_omega, preverror_omega;
int deltat;

int DC_A_desired, DC_B_desired;
int LeftFor, RightFor = 1;

void setLeftFwd(void);
void setLeftRev(void);
void setRightFwd(void);
void setRightRev(void);
void setDrive(void);


// Initializes the motors
void drive_init(void) { 
	set(DDRB,MOTOR_EN); // Define output pins for Motor Control
}

void setLeftFwd(void)  { if (!check(PINB, 3)) { drive_init(); } set(PORTC, 7);   clear(PORTC, 6); LeftFor = 1;}
void setLeftRev(void)  { if (!check(PINB, 3)) { drive_init(); } clear(PORTC, 7); set(PORTC, 6);   LeftFor = 0;}
void setRightFwd(void) { if (!check(PINB, 3)) { drive_init(); } set(PORTB, 6);   clear(PORTB, 5); RightFor = 1;}
void setRightRev(void) { if (!check(PINB, 3)) { drive_init(); } clear(PORTB, 6); set(PORTB, 5);   RightFor = 0;}
void stop(void)        { clear(DDRB, MOTOR_EN); }

// Set Desired Duty Cycle
void fwd_fast(void)  { DC_A_desired = 100;  DC_B_desired = 100;  setDrive(); }
void fwd_slow(void)  { DC_A_desired = 75;   DC_B_desired = 75;   setDrive(); }
void rev_fast(void)  { DC_A_desired = -100; DC_B_desired = -100; setDrive(); }
void rev_slow(void)  { DC_A_desired = -75;  DC_B_desired = -75;  setDrive(); }
void right(void)     { DC_A_desired = 85;   DC_B_desired = 65;   setDrive(); }
void left(void)      { DC_A_desired = 65;   DC_B_desired = 85;   setDrive(); }
void right_slow(void){ DC_A_desired = 60;   DC_B_desired = 30;   setDrive(); }
void left_slow(void) { DC_A_desired = 30;   DC_B_desired = 60;   setDrive(); }
void right_ip(void)  { DC_A_desired = 70;   DC_B_desired = -60;  setDrive(); }
void left_ip(void)   { DC_A_desired = -60;  DC_B_desired = 70;   setDrive(); }

void setDrive(void) {
	int DC_A_curr, DC_B_curr;
	// determine current duty cycle
	if (LeftFor) { DC_A_curr = OCR4A*100.0/255; }
	else { DC_A_curr = 100 - OCR4A*100.0/255; }
	if (RightFor) { DC_B_curr = OCR4B*100.0/255; }
	else { DC_B_curr = 100 - OCR4B*100.0/255;  }
	// low pass filter the duty cycles to determine desired duty cycle
	DC_A_curr = (int) (DC_A_curr * DRIVE_ALPHA + (1 - DRIVE_ALPHA) * DC_A_desired);
	DC_B_curr = (int) (DC_B_curr * DRIVE_ALPHA + (1 - DRIVE_ALPHA) * DC_B_desired);
	// determine whether bot is going forward or reverse, set OCR values
	if (DC_A_curr >= 0) { setLeftFwd(); OCR4A = DC_A_curr * 255 / 100.0; }
	else if (DC_A_curr < 0) { setLeftRev(); OCR4A = 255 + DC_A_curr * 255 / 100.0; }
	if (DC_B_curr >= 0) { setRightFwd(); OCR4B = DC_B_curr * 255 / 100.0; }
	else if (DC_B_curr < 0) { setRightRev(); OCR4B = 255 + DC_B_curr * 255 / 100.0; }

	// m_usb_tx_string("\nOCR4A:");
	// m_usb_tx_int(OCR4A);
	// m_usb_tx_string("\tOCR4B:");
	// m_usb_tx_int(OCR4B);
}

// Test Code to test Motor Controller
void drive_test(void) {
	int i = 500;
	while ( i > 0) {
		rev_fast();  i--;
		
	}
}

// Returns Output of PID comparison on Omega
double thetaPID(double desiredTheta) {
	double error, output;
	error = desiredTheta - theta;
	integral_theta += error * deltat;
	output = K_P * error + K_I * integral_theta + K_D * (error - preverror_theta)/deltat;
	preverror_theta = error;
	return output;
}

// Returns Output of PID comparison on Omega
double omegaPID(double desiredOmega) {
	double error, output;
	error = desiredOmega - omega;
	integral_omega += error * deltat;
	output = K_P * error + K_I * integral_omega + K_D * (error - preverror_omega)/deltat;
	preverror_theta = error;
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
		if (velX > 0 && posX < maxTraversalX) { 
			goToPoint(maxTraversalX, patrolYVal); 
		} else if (velX <= 0  && posX < minTraversalX) {
			goToPoint(minTraversalX, patrolYVal);
		} else if (velX == 0 && velY == 0) { rev_fast(); }
	} else if (currBot == OFF2) {
		if (velX > 0 && posX < maxTraversalX) { 
			goToPoint(maxTraversalX, -patrolYVal); 
		} else if (velX <= 0  && posX < minTraversalX) {
			goToPoint(minTraversalX, -patrolYVal);
		} else if (velX == 0 && velY == 0) { rev_fast(); }
	}
}

void goToGoal(void) {
	if (hasPuck()) {
		goToPoint(offensiveGoalX, 0); // TODO: Change this to follow a go-to-goal with puck (maintains minimum turning radius)
	} else { gameState = PATROL; }
}

// Go to point (x,y)
int goToPoint(int x, int y) {
	double thetaToPos, theta_temp;
	thetaToPos = (double) (atan2(posY - y, posX - x)) + 3.1416 / 2;
	if (thetaToPos > 3.1416) { thetaToPos = thetaToPos - 3.1416 * 2; }
	theta_temp = theta;
	if (abs(posX - x) < 10 && abs(posY - y) < 10) {
		stop();
		return 1;
	} else if ((theta_temp - thetaToPos) > .5) {
		//m_usb_tx_string("r");
		right();
	} else if ((thetaToPos - theta_temp) > .5) {
		//m_usb_tx_string("l");
		left();
	} else if (velX == 0 && velY == 0) { 
		rev_fast();
	} else { fwd_fast(); }
	// m_usb_tx_string("\nx:");
	// m_usb_tx_int((int) (posX));
	// m_usb_tx_string("\ty:");
	// m_usb_tx_int((int) (posY));
	// m_usb_tx_string("\ttheta:");
	// m_usb_tx_int((int) (theta_temp*1000));
	// m_usb_tx_string("\ttheta2goal:");
	// m_usb_tx_int((int) (thetaToPos*1000));
	// m_usb_tx_string("\tdiff:");
	// m_usb_tx_int((int) ((theta_temp - thetaToPos)*1000));
	return 0;
}

