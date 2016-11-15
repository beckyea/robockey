#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_rf.h"

#define RIGHT 0
#define LEFT 1

#define MOTOR PORTB
#define MOTOR_EN 0 
#define LMOTOR_D 1 
#define RMOTOR_D 2

void timer1_init(void);
void init(void);

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
	}

}

void timer1_init(void){
	//Timer 1 config
	set(DDRB,6); //Set B6 to output
	set(TCCR1B,WGM13); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,WGM12); // ^
	set(TCCR1A,WGM11); // ^
	set(TCCR1A,WGM10); // ^
	set(TCCR1B,CS12); // set timer prescaler to 1024
	clear(TCCR1B,CS11);
	set(TCCR1B,CS10);
	set(TCCR1A,COM1B1); //Set at OCR1B, clear at rollover
	set(TCCR1A,COM1B0); // ^
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 10000; // Default frequency: 800Hz
	OCR1B = 5000; // ^ duty cycle: 50%
}

void init(void){ 
	set(DDRB,MOTOR_EN); // Define output pins for Motor Control
	set(DDRB,LMOTOR_D);
	set(DDRB,RMOTOR_D);
	set(DDRB,6);
	sei();//enable global interrupts
	m_clockdivide(0);
}

void fwd(void){
	set(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
}

void rev(void){
	clear(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
}

void right(void){
	set(MOTOR,LMOTOR_D);
	clear(MOTOR,RMOTOR_D);
}

void left(void){
	clear(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
}

void stop(void){
	clear(MOTOR,MOTOR_EN);
	OCR1B = OCR1A;
}

ISR(TIMER1_COMPA_vect){    //PWM signal goes low 
	m_red(TOGGLE);
	clear(MOTOR,MOTOR_EN);
	clear(MOTOR,6);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	set(MOTOR,MOTOR_EN);
	set(MOTOR,6);
}


