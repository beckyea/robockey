/**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
* TO DO - CONVERT THE DRIVE FUNCTIONS TO MACROS
*/

#include "vals.h"
#include "initialization.h"
#include <math.h>

#define MOTOR_PORT PORTB
#define LMOTOR_D 1 
#define RMOTOR_D 2


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

// Go to point (x,y)
int goToPoint(int x, int y) {
	double thetaToPos, theta_temp;
	thetaToPos = (double) (atan2(posY - y, posX - x)) - 3.1416/2;
	if (thetaToPos < - 3.1416) { thetaToPos = thetaToPos + 3.1416*2; }
	theta_temp = theta;
	if (abs(posX - x) < 10 && abs(posY - y) < 10) {
		stop();
		return 1;
	} else if (abs(theta_temp - thetaToPos) > .5 ) {
		right();
	//} else if (thetaToPos - theta_temp > .1) {
		//left();
	} else { fwd(); }
	m_usb_tx_int((int) (posX));
	m_usb_tx_string("\t");
	m_usb_tx_int((int) (posY));
	m_usb_tx_string("\t");
	m_usb_tx_int((int) (theta_temp*1000));
	m_usb_tx_string("\t");
	m_usb_tx_int((int) (thetaToPos*1000));
	m_usb_tx_string("\t");
	m_usb_tx_int((int) ((theta_temp - thetaToPos)*100));
	m_usb_tx_string("\n");
	return 0;
}

