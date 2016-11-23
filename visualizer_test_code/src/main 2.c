
#include "m_general.h"
#include "m_usb.h"


void init(void);
unsigned int buffer1[10] = { 1000, 1, 50, 100, 120, 0, 0, 0, 0, 0 };
unsigned int buffer2[10] = { 1000, 2, 150, 70, 120, 0, 0, 0, 0, 0 };
unsigned int buffer3[10] = { 1000, 3, 75, 30, 120, 0, 0, 0, 0, 0 };

int main(void) {
	init();
	while(1) {
		m_usb_tx_uint(buffer1[0]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[1]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[2]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[3]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[4]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[5]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[6]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[7]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[8]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer1[9]); m_usb_tx_string(","); m_usb_tx_push();

		m_usb_tx_uint(buffer2[0]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[1]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[2]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[3]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[4]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[5]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[6]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[7]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[8]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer2[9]); m_usb_tx_string(","); m_usb_tx_push();

		m_usb_tx_uint(buffer3[0]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[1]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[2]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[3]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[4]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[5]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[6]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[7]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[8]); m_usb_tx_string(","); m_usb_tx_push();
		m_usb_tx_uint(buffer3[9]); m_usb_tx_string(","); m_usb_tx_push();
	}
}

void init(void) {
	m_clockdivide(6);
	m_usb_init(); 
	m_red(ON);
	while (!m_usb_isconnected()) {

	}
	m_green(ON);
	m_red(OFF);
}
