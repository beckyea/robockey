/**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
* TO DO - CONVERT THE DRIVE FUNCTIONS TO MACROS
*/

#include "initialization.h"
#include "vals.h"
#include "m_general.h"
#include <stdlib.h>

#define MOTOR_PORT PORTB
#define LMOTOR_D 1 
#define RMOTOR_D 2
int goToPoint(int x, int y);
double thetaThreshold = 0.2;

// Initializes the motors
void drive_init(void){ 
	set(DDRB,MOTOR_EN); // Define output pins for Motor Control
	set(DDRB,LMOTOR_D);
	set(DDRB,RMOTOR_D);
}

// Drive Forward
void fwd(void){
	set(PORTB,1); //pin D7 (A1 on H bridge)
	set(PORTB,2); //pin D8 (B1 on H bridge)
	clear(PORTB,4); //pin D4 (A2 on H bridge)
	clear(PORTB,3); //pin D9 (B2 on H bridge)
}

// Drive Reverse
void rev(void){
	clear(PORTB,1); //pin D7 (A1 on H bridge)
	clear(PORTB,2); //pin D8 (B1 on H bridge)
	set(PORTB,4); //pin D4 (A2 on H bridge)
	set(PORTB,3); //pin D9 (B2 on H bridge)
}

// Turn Right
void right(void){
	clear(PORTB,1); //pin D7 (A1 on H bridge)
	set(PORTB,2); //pin D8 (B1 on H bridge)
	set(PORTB,4); //pin D4 (A2 on H bridge)
	clear(PORTB,3); //pin D9 (B2 on H bridge)
}

// Turn Left
void left(void){
	set(PORTB,1); //pin D7 (A1 on H bridge)
	clear(PORTB,2); //pin D8 (B1 on H bridge)
	clear(PORTB,4); //pin D4 (A2 on H bridge)
	set(PORTB,3); //pin D9 (B2 on H bridge)
}

// Stop Bot
void stop(void){
	clear(PORTB,MOTOR_EN);
	clear(PORTB,6);
}

// Turns in place by a certain number of radians
void turnInPlaceByTheta(double radians) {
	if (radians < 0) { radians += 2 * 3.1416; }
	double endTheta = theta + radians;
	if (endTheta > 2 * 3.1416) { endTheta -= 2 * 3.1416; }
	if (radians < 3.1416) {
		while (abs(endTheta - theta) > thetaThreshold) { right(); }
	}  else {
		while (abs(endTheta - theta) > thetaThreshold) { left(); }
	}
}

// Turns in place to reach a given orientation in radians
void turnInPlaceToTheta(double endTheta) {
	if (endTheta < 0) { endTheta += 2 * 3.1416; }
	if (endTheta < 3.1416) {
		while (abs(endTheta - theta) > thetaThreshold) { right(); }
	}  else {
		while (abs(endTheta - theta) > thetaThreshold) { left(); }
	}
}

void celebrate(void) {
	right();
	m_wait(1000);
	stop();
}


void patrol(void) {

}

void goToPuck(void) {
	if (puckAngle < .08 || puckAngle > 6.2) { fwd(); }
	else if (puckAngle < 3.1416) { right(); }
	else { left(); }
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
	} else { fwd(); }
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

