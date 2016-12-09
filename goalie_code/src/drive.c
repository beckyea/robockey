 /**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
*/

#include "initialization.h"
#include "vals.h"
#include "m_general.h"
#include "drive.h"
#include <stdlib.h>

#define MOTOR_PORT PORTB

#define DRIVE_ALPHA 0.2

int DC_A_desired, DC_B_desired;

void setToLeft(void);
void setToRight(void);
void setDrive(void);

int ToLeft;
char patrolDirection; // 0 to left, 1 to right


// Initializes the motors
void drive_init(void) { 
	set(DDRD,MOTOR_EN); // Define output pins for Motor Control
	patrolDirection = 1;
}

void setToLeft(void)   { if (!check(PORTB, 3)) { drive_init(); } set(PORTC, 7); clear(PORTC, 6); set(PORTB, 5); clear(PORTB, 6); ToLeft = 1; }
void setToRight(void)  { if (!check(PORTB, 3)) { drive_init(); } clear(PORTC, 7); set(PORTC, 6); clear(PORTB, 5); set(PORTB, 6);  ToLeft = 0; }
void stop(void)        { clear(DDRD, MOTOR_EN); }

// Set Desired Duty Cycle
void right(void) { 
	if (abs(posY) < goalRange) {
		DC_A_desired = 100;   
		DC_B_desired = -100; 
		setDrive(); 
	} else { stop(); }
}
void left(void) { 
	if (abs(posY) < goalRange) {
		DC_A_desired = -100;
		DC_B_desired = 100;
		setDrive(); 
	} else { stop(); }
}

void setDrive(void) {
	int DC_A_curr;
	// determine current duty cycle
	if (ToLeft) { DC_A_curr = OCR4A*100/255; }
	else { DC_A_curr = 100 - OCR4A*100/255; }
	// low pass filter the duty cycles to determine desired duty cycle
	DC_A_curr = (int) (DC_A_curr * DRIVE_ALPHA + (1 - DRIVE_ALPHA) * DC_A_desired);
	// determine whether bot is going forward or reverse, set OCR values
	if (DC_A_curr > 0) { setToLeft(); OCR4A = DC_A_curr * 255 / 100; }
	else if (DC_A_curr < 0) { setToRight(); OCR4A = 255 + DC_A_curr * 255 / 100; }
	else { setToLeft(); OCR4A = 0; }
	OCR4B = 255 - OCR4A;
}

void goToCenter(void) {
	if (posX == 0 && posY == 0) { stop(); }
	else if ((posX < 0 && posY > 0) || (posX > 0 && posY < 0)) { right(); }
	else { left(); }
}

void patrol(void) {
	if (posX < 0 && posY >= goalRange || posX > 0 && posY <= -goalRange) { patrolDirection = 1; }
	else { patrolDirection = 0; }
	if (patrolDirection) { left(); }
	else { right(); }
}

// Test Code to test Motor Controller
void drive_test(void) {
	left(); m_wait(500); right(); m_wait(500);
}
