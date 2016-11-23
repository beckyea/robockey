#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
#include "m_wii.h"
#include <math.h>

#define MOTOR PORTB
#define MOTOR_EN 0 
#define LMOTOR_D 1 
#define RMOTOR_D 2

void fwd(void);
void rev(void);
void left(void);
void right(void);
void stop(void);

void fwd(void){
	set(MOTOR,LMOTOR_D);
	set(MOTOR,RMOTOR_D);
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