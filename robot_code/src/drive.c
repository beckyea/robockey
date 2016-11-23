/**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
* TO DO - CONVERT THE DRIVE FUNCTIONS TO MACROS
*/

#include "vals.h"
#include "initialization.h"
#include <math.h>

#define MOTOR_PORT PORTB
#define MOTOR_EN 0 
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
	set(MOTOR_PORT, LMOTOR_D);
	clear(MOTOR_PORT, RMOTOR_D);
}

// Drive Reverse
void rev(void){
	clear(MOTOR_PORT, LMOTOR_D);
	clear(MOTOR_PORT, RMOTOR_D);
}

// Turn Right
void right(void){
	set(MOTOR_PORT, LMOTOR_D);
	clear(MOTOR_PORT, RMOTOR_D);
}

// Turn Left
void left(void){
	clear(MOTOR_PORT, LMOTOR_D);
	set(MOTOR_PORT, RMOTOR_D);
}

// Stop Bot
void stop(void){
	clear(MOTOR_PORT, MOTOR_EN);
	OCR1B = OCR1A;
}

// Go to point (x,y)
void goToPoint(int x, int y) {
	double thetaToPos;
	thetaToPos = atan2(y - posY, x - posX);
	if (theta - thetaToPos < 1) {
		right();
	} else if (thetaToPos - theta > -1) {
		left();
	} else if (x != posX || y != posY) { 
		fwd(); 
	} else { stop(); }

}

ISR(TIMER1_COMPA_vect){    //PWM signal goes low 
	m_red(TOGGLE);
	clear(MOTOR_PORT, MOTOR_EN);
	clear(MOTOR_PORT, 6);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	set(MOTOR_PORT, MOTOR_EN);
	set(MOTOR_PORT, 6);
}
