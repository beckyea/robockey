#include "avr/interrupt.h"
#include "m_general.h"
#include "vals.h"

// Initializes Timer 1 for Global Time Tracking
void clock_init(void) {
	clear(TCCR1B, CS12); set(TCCR1B, CS11); set(TCCR1B, CS10); // set clock scale to /64 --> running at 250kHz
	clear(TCCR1B, WGM13); set(TCCR1B, WGM12); clear(TCCR1A, WGM11); clear(TCCR1A, WGM10); // mode 4, up to OCR1A
	set(TIMSK1, OCIE1A); // sets an interrupt when OCR1A is reached
	OCR1A = 250;
}

void clock_reset(void) {
	time = 0;
	TCNT1 = 0;
}

// Interrupt Called to incr global time value
ISR(TIMER1_COMPA_vect) {
	time = time + 1;
}