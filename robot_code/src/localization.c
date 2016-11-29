#include "vals.h"
#include "localization.h"
#include "m_usb.h"
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SQ(x) ((x) * (x))

#define XSHIFT 535
#define YSHIFT 380
double PI = 3.14159;
double scaleFactor = 0.31;

unsigned int blobs[12];
char recievedWii;

void set4Pts(int x[], int y[]);
void set3Pts(int x[], int y[]);
void findOrientation();
void readStars();


/* Reads the Wii. Begins execution of localization */
char loc_readWii() {
	recievedWii = m_wii_read(blobs);
	if (recievedWii) { 
		readStars(); 
		// UNCOMMENT BELOW TO USE VISUALIZER
		// m_usb_tx_uint(5555); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[0]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[1]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[3]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[4]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[6]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[7]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[9]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_uint(blobs[10]); m_usb_tx_string(","); m_usb_tx_push();
		// m_usb_tx_string("\n"); m_usb_tx_push();
	}
	return recievedWii;
}

/* Reads the stars from the blobs array. 
   Determines number of stars seen and executes appropriate function. */
void readStars() {
	int valCount = 0;
	int x_vals[4];
	int y_vals[4];
	int i;
	for (i = 0; i < 4; i++) {
		if (blobs[i * 3] != 1023 && blobs[i * 3 + 1] != 1023) {
			x_vals[valCount] = (int)(blobs[i * 3] - XSHIFT);
			y_vals[valCount] = (int)(blobs[i * 3 + 1] - YSHIFT);
			valCount++;
		}
	}
	if (valCount == 4) { set4Pts(x_vals, y_vals); }
	else if (valCount == 3) { set3Pts(x_vals, y_vals); m_usb_tx_string("\n");}
	else if (valCount == 2) { }//m_usb_tx_int(2); m_usb_tx_string("\n");}
	else if (valCount == 3) { }//m_usb_tx_int(1); m_usb_tx_string("\n");}
	else { }//m_usb_tx_int(0); }
}

/* Sets the center of the field in the local frame of the robot
   Defines the robot's distance (unscaled) from the center of the frame */
void set4Pts(int x[], int y[]) {
	long minDist, maxDist, d12, d13, d14, d23, d24, d34;
	int B, D; B = D = 0;
	int centerx, centery;
	
	d12 = SQ(x[1] - x[0]) + SQ(y[1] - y[0]);
	d13 = SQ(x[2] - x[0]) + SQ(y[2] - y[0]);
	d14 = SQ(x[3] - x[0]) + SQ(y[3] - y[0]);
	d23 = SQ(x[2] - x[1]) + SQ(y[2] - y[1]);
	d24 = SQ(x[3] - x[1]) + SQ(y[3] - y[1]);
	d34 = SQ(x[3] - x[2]) + SQ(y[3] - y[2]);
	// calculate maximum distance between blobs
	maxDist =  d12     > d13 ?  d12     : d13;
	maxDist =  maxDist > d14 ?  maxDist : d14;
	maxDist =  maxDist > d23 ?  maxDist : d23;
	maxDist =  maxDist > d24 ?  maxDist : d24;
	maxDist =  maxDist > d34 ?  maxDist : d34;
	// calcualte minimum distance between blobs
	minDist =  d12     < d13 ?  d12     : d13;
	minDist =  minDist < d14 ?  minDist : d14;
	minDist =  minDist < d23 ?  minDist : d23;
	minDist =  minDist < d24 ?  minDist : d24;
	minDist =  minDist < d34 ?  minDist : d34;
	// compare distance values to find B & D
	if (maxDist == d12) {
    	if (minDist == d13 || minDist == d14) { B = 0; D = 1; } 
    	else { B = 1; D = 0; }
	} else if (maxDist == d13) {
		if (minDist == d12 || minDist == d14) { B = 0; D = 2; } 
		else { B = 2; D = 0; }
    } else if (maxDist == d14) {
    	if (minDist == d13 || minDist == d12) { B = 0; D = 3; }
    	else { B = 3; D = 0; }
    } else if (maxDist == d23) {
    	if (minDist == d12 || minDist == d24) { B = 1; D = 2; }
    	else { B = 2; D = 1; }
    } else if (maxDist == d24) {
    	if (minDist == d12 || minDist == d23) { B = 1; D = 3; }
    	else { B = 3; D = 1; }
    } else if (maxDist == d34) {
    	if (minDist == d13 || minDist == d23) { B = 2; D = 3; }
    	else { B = 3; D = 2;}
    }
    centerx = (x[B] + x[D]) / 2;
	centery = (y[B] + y[D]) / 2;
	findOrientation(x[B], y[B], x[D], y[D], centerx, centery);
}

void set3Pts(int x[], int y[]) {
	long minDist, maxDist, d12, d13, d23;
	int A, B, C, D; A = B = C = D = 0;
	int centerx, centery;
	d12 = SQ(x[1] - x[0]) + SQ(y[1] - y[0]);
	d13 = SQ(x[2] - x[0]) + SQ(y[2] - y[0]);
	d23 = SQ(x[2] - x[1]) + SQ(y[2] - y[1]);
	// calculate maximum distance between blobs
	maxDist =  d12     > d13 ?  d12     : d13;
	maxDist =  maxDist > d23 ?  maxDist : d23;
	// calcualte minimum distance between blobs
	minDist =  d12     < d13 ?  d12     : d13;
	minDist =  minDist < d23 ?  minDist : d23;
	double ratio = (double) maxDist / minDist;
	if (ratio < 2.0) { // B missing, Max: CD, Min: AD
		if (maxDist == d12) {
			if (minDist == d13) { A = 2; C = 1; D = 0; }
			else { A = 2; C = 0; D = 1; }
		} else if (maxDist == d13) {
			if (minDist == d12) { A = 1; C = 2; D = 0; }
			else { A = 1; C = 0; D = 2; }
		} else if (maxDist == d23) {
			if (minDist == d12) { A = 0; C = 2; D = 1; }
			else { A = 0; C = 1; D = 2; }
		}
		int Mx, My;
		Mx = x[C] + (x[C] - x[A]) * 0.5245;
		My = y[C] + (y[C] - y[A]) * 0.5245;
		centerx = Mx - (x[D] + (x[D] - Mx) * 0.273);
		centery = My - (y[D] + (y[D] - My) * 0.273);
		m_usb_tx_string("B");
		findOrientation(centerx, centerx, x[D], y[D], centerx, centery);
	} else if (ratio > 4.0) { // A missing, Max: BD, Min: BC
		if (maxDist == d12) {
			if (minDist == d13) { B = 0; D = 1; }
			else { B = 1; D = 0; }
		} else if (maxDist == d13) {
			if (minDist == d12) { B = 0; D = 2; }
			else { B = 2; D = 0; }
		} else if (maxDist == d23) {
			if (minDist == d13) { B = 2; D = 1; }
			else { B = 1; D = 2; }
		}
		centerx = (x[B] + x[D]) / 2;
		centery = (y[B] + y[D]) / 2;
		m_usb_tx_string("A");
		findOrientation(x[B], y[B], x[D], y[D], centerx, centery);
	} else if ((long)(maxDist*maxDist/minDist) < 2200) { // D missing, Max: AC, Min: BC
		if (maxDist == d12) {
			if (minDist == d13) { A = 1; B = 2; C = 0; }
			else { A = 0; B = 2; C = 1; }
		} else if (maxDist == d13) {
			if (minDist == d12) { A = 2; B = 1; C = 0; }
			else { A = 0; B = 1; C = 2; }
		} else if (maxDist == d23) {
			if (minDist == d12) { A = 2; B = 0; C = 1; }
			else { A = 1; B = 0; C = 2; }
		}
		int Mx, My;
		Mx = x[C] + (x[C] - x[A]) * 0.5245;
		My = y[C] + (y[C] - y[A]) * 0.5245;
		centerx = Mx - (x[B] + (x[B] - Mx) * 1.604);
		centery = My - (y[B] + (y[B] - My) * 1.604);
		findOrientation(centerx, centerx, x[D], y[D],  centerx, centery);
		m_usb_tx_string("D");
	} else { // C missing, Max: BD, Min: AB
		if (maxDist == d12) { 
			if (minDist == d13) { B = 0; D = 1; }
			else { B = 1; D = 0; }
		} else if (maxDist == d13) {
			if (minDist == d12) { B = 0; D = 2; }
			else { B = 2; D = 0; }
		} else if (maxDist == d23) {
			if (minDist == d13) { B = 2; D = 1; }
			else { B = 1; D = 2; }
		}
		centerx = (x[B] + x[D]) / 2;
		centery = (y[B] + y[D]) / 2;
		m_usb_tx_string("C");
		findOrientation(x[B], y[B], x[D], y[D], centerx, centery);
	}
	m_usb_tx_string("\n");
}


/* Determines orientation of the puck given two input positions. */
void findOrientation(int Bx, int By, int Dx, int Dy, int centerx, int centery) {
	double theta2, dist2center;
	theta = atan2((Dy - By), (Dx - Bx)) - PI/2;
	if (theta < -PI) { theta += 2 * PI; }
	theta2 = -atan2(centerx, centery) - PI/2;
	if (theta2 < -PI) { theta2 += 2 * PI; }
	dist2center = sqrt((centerx/10.0)*(centerx/10.0) + (centery/10.0)*(centery/10.0)) * 10.0;
	theta2 = theta - theta2 - PI;
	if (theta2 < -PI) { theta2 += 2 * PI; }
	posX = -(int) (dist2center*cos(theta2) * scaleFactor);
	posY = -(int) (dist2center*sin(theta2) * scaleFactor);
	// m_usb_tx_int((int) (posX));
	// m_usb_tx_string("\t");
	// m_usb_tx_int((int) (posY));
	// m_usb_tx_string("\n");
}

/* Gets x as seen in unsigned bits */
unsigned int loc_getX() { return posX + 115; }

/* Gets y as seen in unsigned bits */
unsigned int loc_getY() { return posY + 60; }

/* Gets theta as seen in unsigned bits */
unsigned int loc_getT() { return (unsigned int) (theta * 100); }

/* Determines side of court */
char loc_getSide() { 
	if (posX < 0 && posX > -115) { return 1; } // Left side
	else if (posX < 115) { return 2; } // Right side
	else { return 0; } // Error
}