#include <stdio.h>
#include <math.h>
#include "m_wii.h"

int posX, posY = 500;
double theta;
unsigned int blobs[12];
char recievedWii;

void set4Pts(unsigned int x[], unsigned int y[]);
void set3Pts(unsigned int x[], unsigned int y[]);
double findOrientation();
void setPosition();
char loc_readWii();
void readStars();
unsigned int loc_getX();
unsigned int loc_getY();
unsigned int loc_getT();
char loc_getSide();


/* Reads the Wii. Begins execution of localization */
char loc_readWii() {
	recievedWii = m_wii_read(blobs);
	if (recievedWii) { 
		readStars(); 
		// TODO: Something to send out position of stars
	}
	return recievedWii;
}

/* Reads the stars from the blobs array. 
   Determines number of stars seen and executes appropriate function. */
void readStars() {
	int valCount;
	unsigned int x_vals[4];
	unsigned int y_vals[4];
	int i;
	for (i = 0; i < 4; i++) {
		if (blobs[i * 3] != 1023 && blobs[i * 3 + 1] != 1023) {
			x_vals[valCount] = blobs[i * 3] - 512;
			y_vals[valCount] = blobs[i * 3 + 1] - 384;
			valCount++;
		}
	}
	if (valCount == 0) { set4Pts(x_vals, y_vals); }
	else if (valCount == 1) { set3Pts(x_vals, y_vals); }
	else if (valCount == 2) { m_usb_tx_string('Sensed 2 stars'); }
	else if (valCount == 3) { m_usb_tx_string('Sensed 1 star'); }
	else { m_usb_tx_string('Sensed no stars'); }
}

/* Sets the center of the field in the local frame of the robot
   Defines the robot's distance (unscaled) from the center of the frame */
void set4Pts(unsigned int x[], unsigned int y[]) {
	long minDist, maxDist, d12, d13, d14, d23, d24, d34;
	char B, D;
	unsigned int centerx, centery;
	
	d12 = pow((x[2] - x[1]), 2) + pow((y[2] - y[1]), 2);
	d13 = pow((x[3] - x[1]), 2) + pow((y[3] - y[1]), 2);
	d14 = pow((x[4] - x[1]), 2) + pow((y[4] - y[1]), 2);
	d23 = pow((x[3] - x[2]), 2) + pow((y[3] - y[2]), 2);
	d24 = pow((x[4] - x[2]), 2) + pow((y[4] - y[2]), 2);
	d34 = pow((x[4] - x[3]), 2) + pow((y[4] - y[3]), 2);
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
    	if (minDist == d13 || minDist == d14) { B = 1; D = 2; } 
    	else { B = 2; D = 1; }
	} else if (maxDist == d13) {
		if (minDist == d12 || minDist == d14) { B = 1; D = 3; } 
		else { B = 3; D = 1; }
    } else if (maxDist == d14) {
    	if (minDist == d13 || minDist == d12) { B = 1; D = 4; }
    	else { B = 4; D = 1; }
    } else if (maxDist == d23) {
    	if (minDist == d12 || minDist == d24) { B = 2; D = 3; }
    	else { B = 3; D = 2; }
    } else if (maxDist == d24) {
    	if (minDist == d12 || minDist == d23) { B = 2; D = 4; }
    	else { B = 4; D = 2; }
    } else if (maxDist == d34) {
    	if (minDist == d13 || minDist == d23) { B = 3; D = 4; }
    	else { B = 4; D = 3;}
    }
    centerx = (x[B] + x[D]) / 2;
	centery = (y[B] + y[D]) / 2;
	theta = findOrientation(x[B], y[B], x[D], y[D]);
	setPosition(centerx, centery);
}

void set3Pts(unsigned int x[], unsigned int y[]) {
	long minDist, maxDist, d12, d13, d23;
	char A, B, C, D;
	unsigned int centerx, centery;
	d12 = pow((x[2] - x[1]), 2) + pow((y[2] - y[1]), 2);
	d13 = pow((x[3] - x[1]), 2) + pow((y[3] - y[1]), 2);
	d23 = pow((x[3] - x[2]), 2) + pow((y[3] - y[2]), 2);
	// calculate maximum distance between blobs
	maxDist =  d12     > d13 ?  d12     : d13;
	maxDist =  maxDist > d23 ?  maxDist : d23;
	// calcualte minimum distance between blobs
	minDist =  d12     < d13 ?  d12     : d13;
	minDist =  minDist < d23 ?  minDist : d23;
	double ratio = (double) maxDist / minDist;
	if (ratio < 2.0) { // B missing, Max: CD, Min: AD
		if (maxDist == d12) {
			if (minDist == d13) { A = 3; C = 2; D = 1; }
			else { A = 3; C = 1; D = 2; }
		} else if (maxDist == d13) {
			if (minDist == d12) { A = 2; C = 3; D = 1; }
			else { A = 2; C = 1; D = 3; }
		} else if (maxDist == d23) {
			if (minDist == d12) { A = 1; C = 3; D = 2; }
			else { A = 1; C = 2; D = 3; }
		}
		int Mx, My;
		Mx = x[C] + (x[C] - x[A]) * 0.5245;
		My = y[C] + (y[C] - y[A]) * 0.5245;
		centerx = Mx - (x[D] + (x[D] - Mx) * 0.273);
		centery = My - (y[D] + (y[D] - My) * 0.273);
		theta = findOrientation(centerx, centerx, x[D], y[D]);
	} else if (ratio > 4.0) { // A missing, Max: BD, Min: BC
		if (maxDist == d12) {
			if (minDist == d13) { B = 1; D = 2; }
			else { B = 2; D = 1; }
		} else if (maxDist == d13) {
			if (minDist == d12) { B = 1; D = 3; }
			else { B = 3; D = 1; }
		} else if (maxDist == d23) {
			if (minDist == d13) { B = 3; D = 2; }
			else { B = 2; D = 3; }
		}
		centerx = (x[B] + x[D]) / 2;
		centery = (y[B] + y[D]) / 2;
		theta = findOrientation(x[B], y[B], x[D], y[D]);
	} else if (ratio < 3.2) { // D missing, Max: AC, Min: BC
		if (maxDist == d12) {
			if (minDist == d13) { A = 2; B = 3; C = 1; }
			else { A = 1; B = 3; C = 2; }
		} else if (maxDist == d13) {
			if (minDist == d12) { A = 3; B = 2; C = 1; }
			else { A = 1; B = 2; C = 3; }
		} else if (maxDist == d23) {
			if (minDist == d12) { A = 3; B = 1; C = 2; }
			else { A = 2; B = 1; C = 3; }
		}
		int Mx, My;
		Mx = x[C] + (x[C] - x[A]) * 0.5245;
		My = y[C] + (y[C] - y[A]) * 0.5245;
		centerx = Mx - (x[B] + (x[B] - Mx) * 1.604);
		centery = My - (y[B] + (y[B] - My) * 1.604);
		theta = findOrientation(centerx, centerx, x[D], y[D]);
	} else { // C missing, Max: BD, Min: AC
		if (maxDist == d12) { 
			if (minDist == d13) { B = 1; D = 2; }
			else { B = 2; D = 1; }
		} else if (maxDist == d13) {
			if (minDist == d12) { B = 1; D = 3; }
			else { B = 3; D = 1; }
		} else if (maxDist == d23) {
			if (minDist == d13) { B = 3; D = 2; }
			else { B = 2; D = 3; }
		}
		centerx = (x[B] + x[D]) / 2;
		centery = (y[B] + y[D]) / 2;
		theta = findOrientation(x[B], y[B], x[D], y[D]);
	}
	setPosition(centerx, centery);
}


/* Determines orientation of the puck given two input positions. */
double findOrientation(int Bx, int By, int Dx, int Dy) {
	return atan2((Bx - Dx), (By - Dy));
}

/* Sets position of the robot in coordinates relative to the global frame */
void setPosition(int centerx, int centery) {
	posX = -(centerx * cos(theta) - centery * sin(theta));
	posY = (centerx * sin(theta) + centery * cos(theta));
}

/* Gets x as seen in unsigned bits */
unsigned int loc_getX() { return posX + 115; }

/* Gets y as seen in unsigned bits */
unsigned int loc_getY() { return posY + 60; }

/* Gets theta as seen in unsigned bits */
unsigned int loc_getT() { return theta * 100; }

/* Determines side of court */
char loc_getSide() { 
	if (posX < 0 && posX > -115) { return 1; } // Left side
	else if (posX < 115) { return 2; } // Right side
	else { return 0; } // Error
}
