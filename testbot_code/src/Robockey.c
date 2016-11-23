#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_wii.h"
#include "m_rf.h"

#define RIGHT 0
#define LEFT 1

#define CHANNEL 1 //Channel to receive game commands from Fiene
#define RXADDRESS 
#define TXADDRESS 
#define PACKET_LENGTH 10

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

//Game Commands
#define COMM_TEST 0xA0 // Flash Positioning LEDs
#define PLAY 0xA1 //LEDs must illuminate, robots must move
#define GOAL_R 0xA2
#define GOAL_B 0xA3
#define PAUSE 0xA4  //Robots must stop within 3 seconds
#define HALF 0xA6
#define GAME_OVER 0xA7

volatile bool play = false;

void timer1_init(void);
void init(void);

int main(void){
	init();
	m_usb_init(); // initialize usb
	timer1_setup(); // setup timer 1

}

void timer1_setup(void){
	//Timer 1 config
	set(DDRB,6); //Set B6 to output
	set(TCCR1B,WGM13); //Set to mode 15 (up to OCR1A)
	set(TCCR1B,WGM12); // ^
	set(TCCR1A,WGM11); // ^
	set(TCCR1A,WGM10); // ^
	set(TCCR1A,COM1B1); //Set at OCR1B, clear at rollover
	set(TCCR1A,COM1B0); // ^
	set(TIMSK1,OCIE1A); //Enable interrupt when TCNT1 = OCR1A
	set(TIMSK1,OCIE1B); //Enable interrupt when TCNT1 = OCR1B
	OCR1A = 20000; // Default frequency: 800Hz
	OCR1B = 18000; // ^ duty cycle: 10%
}

void init(void){ 
	set(DDRB,MOTOR_EN); // Define output pins for Motor Control
	set(DDRB,LMOTOR_D);
	set(DDRB,RMOTOR_D);
	
	m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH); // Setup wireless module
	sei();//enable global interrupts
	int b = m_wii_open(); // initialize m_wii
}


ISR(INT2_vect) {
	m_rf_read(buffer, PACKET_LENGTH);	
	if (buffer[0] == PLAY)
		play = true;
	else if (buffer[0] == PAUSE)
		stop();
}

ISR(TIMER1_COMPA_vect){    //PWM signal goes low 
	clear(MOTOR,MOTOR_EN);
}

ISR(TIMER1_COMPB_vect){   //PWM signal goes high
	set(MOTOR,MOTOR_EN);
}

