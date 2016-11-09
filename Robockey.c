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

void timer1_init(void);

void init()
{ 
	// Define output pins
	set(DDRB,MOTOR_EN);
	set(DDRB,LMOTOR_D);
	set(DDRB,RMOTOR_D);

}

