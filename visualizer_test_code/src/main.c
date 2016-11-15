
#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"


void init();
unsigned int buffer[10];

int main(void) {
	init();
	while(1) {
		m_usb_tx_uint(buffer[0]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[1]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[2]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[3]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[4]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[5]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[6]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[7]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[8]);
		m_usb_tx_string(',');
		m_usb_tx_uint(buffer[9]);
		m_usb_tx_push();


	}
}

void init(void) {
	m_clockdivide(0); // Set system clock prescaler to 1 (16MHz)
	m_bus_init(); //Initialize mBUS pins
	m_usb_init(); 
	buffer = [1, 1, 100, 100, 100, 0, 0, 0, 0, 0]
}
