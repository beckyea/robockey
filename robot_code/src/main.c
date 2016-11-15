
 /* Name: main.c
* Author: <Adnan Jafferjee>
* Copyright: <insert your copyright message here>
* License: <insert your license reference here>
*/
#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "m_wii.h"
#include "math.h"

int xOffset = 0; //
int yOffset = 0;

int x0 = (1024/2)-xOffset; //Offsets for pixel resolution of the Wii Sensor
int y0 = (768/2)-yOffset;

double P0[2]={0,0}; // Hard-coding positions of stars in centimeter coordinate-frame
double P1[2]={0,14.5};
double P2[2]={11.655,8.741};
double P3[2]={0,-14.5};
double P4[2]={-10.563,2.483};
double P_vert = 29.0; //Vertical difference between P1 and P3

unsigned int blobs[12]= {};


//Prototyping subroutines 
void init(void);

int main(void) {
 init();

while(1) {
	char = m_wii_read(blobs)
	double x1 = blobs[0];
	double y1 = blobs[1];
	double x2 = blobs[3];
	double y2 = blobs[4];
	double x3 = blobs[6];
	double y3 = blobs[7];
	double x4 = blobs[9];
	double y4 = blobs[10];

	double QA = {x1-x0, y1-y0};
	double QB = {x2-x0, y2-y0};
	double QC = {x3-x0, y3-y0};
	double QD = {x4-x0, y4-y0};
  

}
}

void init(void) {
 m_clockdivide(0); // Set system clock prescaler to 1 (16MHz)
 m_bus_init(); //Initialize mBUS pins
 // m_rf_open(CHANNEL,RXADDRESS,PACKET_LENGTH); //Open wireless comm.
 m_usb_init(); 
 char blobCheck = m_wii_open(void);


}
