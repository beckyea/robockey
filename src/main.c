/* Name: main.c
 */
#include "m_general.h"
#include "m_usb.h"
#include "m_rf.h"
#include "m_bus.h"
#include <stdio.h>

int main (void) {
	init();
	while (true) {
	}
	return 0;
}

void init(void) {
	m_usb_init();
	m_bus_init();
	sei();
}