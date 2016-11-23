/*
 * Motor Controller Bidirectional Drive.c
 *
 * Created: 11/9/2016 11:30:42 AM
 * Author : julialin06
 */ 

/* Red Motor Controller for Bidirectional Motor Drive
Digital (PWM) Pins from 2-13 
*/

#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"
#define MOTOR_EN 0
#define RIGHT 0
#define LEFT 1

void timer1_init(void);
void init(void);

void test(void);
void fwd(void);
void rev(void);
void left(void);
void right(void);
void stop(void);

int main(void){
	init();
	m_usb_init(); // initialize usb
	timer1_init(); // setup timer 1

	while(1){
		fwd();
		m_wait(7000);
		stop();
		m_wait(15000);
		rev();
		m_wait(7000);
		stop();
		m_wait(15000);
		left();
		m_wait(7000);
		stop();
		m_wait(15000);
		right();
		m_wait(7000);
		stop();
		m_wait(15000);
	}

}

void timer1_init(void){
	//Timer 1 config
	set(TCCR1B,WGM13); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,WGM12); // ^
	set(TCCR1A,WGM11); // ^
	set(TCCR1A,WGM10); // ^
	
	clear(TCCR1B,CS12); // set timer prescaler to /64
	set(TCCR1B,CS11);
	set(TCCR1B,CS10);
	
	set(TCCR1A,COM1B1); //Set at OCR1B, clear at rollover
	set(TCCR1A,COM1B0); // ^
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 10000; // Default frequency: 800Hz
	OCR1B = 5000; // ^ duty cycle: 50%
}

void init(void){
	// Set pins as outputs, what they connect from M2 to the red motor controller
	// set(DDRB,MOTOR_EN); //B0 --> A0
	set(DDRB,1); //B1 --> D7
	set(DDRB,2); //B2 --> D8
	set(DDRB,3); //B3 --> D9
	set(DDRB,4); //B4 --> D4
	set(DDRB,5); //B5 --> D5
	set(DDRB,6); //B6 --> D6
	//set(DDRB,7);

	sei();//enable global interrupts
	m_clockdivide(0);
}

void test(void){
	set(PORTB,1); //pin D7 (A1 on H bridge)
	clear(PORTB,2); //pin D8 (B1 on H bridge)
	clear(PORTB,4); //pin D4 (A2 on H bridge)
	set(PORTB,3); //pin D9 (B2 on H bridge)
}

void fwd(void){
	set(PORTB,1); //pin D7 (A1 on H bridge)
	set(PORTB,2); //pin D8 (B1 on H bridge)
	clear(PORTB,4); //pin D4 (A2 on H bridge)
	clear(PORTB,3); //pin D9 (B2 on H bridge)
}

void rev(void){
	clear(PORTB,1); //pin D7 (A1 on H bridge)
	clear(PORTB,2); //pin D8 (B1 on H bridge)
	set(PORTB,4); //pin D4 (A2 on H bridge)
	set(PORTB,3); //pin D9 (B2 on H bridge)
}

void right(void){
	clear(PORTB,1); //pin D7 (A1 on H bridge)
	set(PORTB,2); //pin D8 (B1 on H bridge)
	set(PORTB,4); //pin D4 (A2 on H bridge)
	clear(PORTB,3); //pin D9 (B2 on H bridge)
}

void left(void){
	set(PORTB,1); //pin D7 (A1 on H bridge)
	clear(PORTB,2); //pin D8 (B1 on H bridge)
	clear(PORTB,4); //pin D4 (A2 on H bridge)
	set(PORTB,3); //pin D9 (B2 on H bridge)
}

void stop(void){
	// clear(PORTB,MOTOR_EN);
	clear(PORTB,6);
}

ISR(TIMER1_COMPA_vect){    //PWM signal goes low
	// clear(PORTB,MOTOR_EN);
	//clear(PORTB,7);
	clear(PORTB,5);
	clear(PORTB,6);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	// set(PORTB,MOTOR_EN);
	//set(PORTB,7);
	set(PORTB,5);
	set(PORTB,6);
}