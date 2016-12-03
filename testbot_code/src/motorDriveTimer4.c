 /**
* Methods to control the robot's drivetrain given input values of where to go
* and how to get there.
* TO DO - CONVERT THE DRIVE FUNCTIONS TO MACROS
*/

#include "m_general.h"
#define motorDutyLeft
#define motorDutyRight

void init_driver(void);
void initMotorTimer(void);
void right(void);
void left(void);
void fwd(void);
void stop(void);
void rev(void);

int main(void) {
	init_driver();
	initMotorTimer():
	while(1){
		fwd();
		m_wait(1000);
		rev();
		m_wait(1000);
		right();
		m_wait(1000);
		left();
		m_wait(1000);
		stop();
	}
}



// Initializes Timer 1, pins for Motor Output
void init_driver(void) {
	m_clockdivide(0); 
	sei(); //Enable Global Interrupts
	set(DDRD,3); //D3 --> A0
	set(DDRC,6); //B1 --> D7 (INA1)
	set(DDRB,6); //B4 --> D4 (INA2)
	set(DDRC,7); //B2 --> D8 (INB1)
	set(DDRB,5); //B3 --> D9 (INB2)
	
	//Disable internal pull-up resistors 
	clear(PORTC,6); //B1 --> D7 (INA1)
	clear(PORTB,6); //B2 --> D8 (INA2)
	clear(PORTC,7); //B3 --> D9 (INB1)
	clear(PORTB,5); //B4 --> D4 (INB2)
	//Set PWM Pins on Motor Driver to +5V (all the time) => D5,D6 (on driver)
}

void initMotorTimer(void){
 	//Timer 4 configure
	set(TCCR4B,CS43); // set timer prescaler to /64
	clear(TCCR4B,CS42); 
	clear(TCCR4B,CS41);
	clear(TCCR4B,CS40);

	clear(TCCR4D,WGM41);  // Count UP to OCR4C
	clear(TCCR4D,WGM40);
	
	set(TCCR4A,PWM4A); //PWM, set at rollover, clear at OCR4A
	set(TCCR4A,COM4A1); //
	clear(TCCR4A,COM4A0); // ^

	set(TCCR4B,PWM4B); //PWM, set at rollover, clear at OCR4B
	set(TCCR4B,COM4B1); //
	clear(TCCR4B,COM4B0); // ^

	set(TIMSK4,OCIE4A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK4,OCIE4B); //Enable interrupt when TCNT1 = OCR1B
	OCR4A = 10000; // Default frequency: 800Hz
	OCR4B = 10000; // ^ duty cycle: 50%
}



// Drive Forward
void fwd(void){
	set(PORTC,7); //pin D7 (A1 on H bridge)
	set(PORTB,6); //pin D8 (B1 on H bridge)
	clear(PORTC,6); //pin D4 (A2 on H bridge)
	clear(PORTB,5); //pin D9 (B2 on H bridge)
}

// Drive Reverse
void rev(void){
	clear(PORTC,7); //pin D7 (A1 on H bridge)
	clear(PORTB,6); //pin D8 (B1 on H bridge)
	set(PORTC,6); //pin D4 (A2 on H bridge)
	set(PORTB,5); //pin D9 (B2 on H bridge)
}

// Turn Right
void right(void){
	set(PORTC,7); //pin D7 (A1 on H bridge)
	clear(PORTB,6); //pin D8 (B1 on H bridge)
	clear(PORTC,6); //pin D4 (A2 on H bridge)
	set(PORTB,5); //pin D9 (B2 on H bridge)
}

// Turn Left
void left(void){
	clear(PORTC,7); //pin D7 (A1 on H bridge)
	set(PORTB,6); //pin D8 (B1 on H bridge)
	set(PORTC,6); //pin D4 (A2 on H bridge)
	clear(PORTB,5); //pin D9 (B2 on H bridge)
}

// Stop Bot
void stop(void){
	clear(TCCR4B,CS43); //Stop PWM by clearing the clock on Timer 4
	clear(TCCR4B,CS42);
	clear(TCCR4B,CS41);
	clear(TCCR4B,CS40);
}