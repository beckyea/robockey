#include <stdio.h>
#include <math.h>

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

struct Position { int x; int y; };

struct Position center, bot, B, D;
double orientation;

void set4Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
void set3Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3);
double findOrientation();
void setPosition();

int main() {
	set4Pts(261, 208, 184, 261, 270, 284, 247, 202);
	//set4Pts(259, 208, 183, 259, 274, 288, 251, 204);

	//set4Pts(492, 330, 473, 368, 567, 367, 513, 402);
	//set3Pts(473, 368, 567, 367, 513, 402); // CASE C
	//set3Pts(492, 330, 473, 368, 567, 367); // CASE A
	printf("x: %d, y: %d, theta: %f\n", bot.x, bot.y, orientation);
}


// Sets the center of the field in the local frame of the robot
// Defines the robot's distance (unscaled) from the center of the frame
void set4Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4) {
	long minDist, maxDist, d12, d13, d14, d23, d24, d34;
	d12 = pow((X2 - X1), 2) + pow((Y2 - Y1), 2);
	d13 = pow((X3 - X1), 2) + pow((Y3 - Y1), 2);
	d14 = pow((X4 - X1), 2) + pow((Y4 - Y1), 2);
	d23 = pow((X3 - X2), 2) + pow((Y3 - Y2), 2);
	d24 = pow((X4 - X2), 2) + pow((Y4 - Y2), 2);
	d34 = pow((X4 - X3), 2) + pow((Y4 - Y3), 2);
	maxDist = max (max (max(d12, d13), max(d14, d23)), max(d24, d34));
	minDist = min (min (min(d12, d13), min(d14, d23)), min(d24, d34));

	// check largest distance values to determine B and D, find center
	if (maxDist == d12) { 
		if (minDist == d13 || minDist == d14) {
			B.x = X1; B.y = Y1; D.x = X2; D.y = Y2;
		} else { D.x = X1; D.y = Y1; B.x = X2; B.y = Y2; }
	} else if (maxDist == d13) { 
		if (minDist == d12 || minDist == d14) {
			B.x = X1; B.y = Y1; D.x = X3; D.y = Y3;
		} else { D.x = X1; D.y = Y1; B.x = X3; B.y = Y3; }
	} else if (maxDist == d14) {
		if (minDist == d13 || minDist == d12) {
			B.x = X1; B.y = Y1; D.x = X4; D.y = Y4;
		} else { D.x = X1; D.y = Y1; B.x = X4; B.y = Y4; }
	} else if (maxDist == d23) {
		if (minDist == d12 || minDist == d24) {
			B.x = X2; B.y = Y2; D.x = X3; D.y = Y3;
		} else { D.x = X2; D.y = Y2; B.x = X3; B.y = Y3; }
	} else if (maxDist == d24) {
		if (minDist == d12 || minDist == d23) {
			B.x = X2; B.y = Y2; D.x = X4; D.y = Y4;
		} else { D.x = X2; D.y = Y2; B.x = X4; B.y = Y4; } 
	} else if (maxDist == d34) {
		if (minDist == d13 || minDist == d23) {
			B.x = X3; B.y = Y3; D.x = X4; D.y = Y4;
		} else { D.x = X3; D.y = Y3; B.x = X4; B.y = Y4; }
	} else { 
		printf("Something is wrong. Largest distance is %ld\n", maxDist);
	}
	printf("d12: %ld, d13: %ld, d14: %ld, d23: %ld, d24: %ld, d34: %ld\n", d12, d13, d14, d23, d24, d34);
	printf("minDist: %ld, maxDist: %ld, Bx: %d, By: %d, Dx: %d, Dy: %d\n", minDist, maxDist, B.x, B.y, D.x, D.y);
	center.x = (B.x + D.x) / 2;
	center.y = (B.y + D.y) / 2;
	orientation = findOrientation(B, D);
	setPosition();
}

void set3Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3) {
	long minDist, maxDist, d12, d13, d23;
	d12 = pow((X2 - X1), 2) + pow((Y2 - Y1), 2);
	d13 = pow((X3 - X1), 2) + pow((Y3 - Y1), 2);
	d23 = pow((X3 - X2), 2) + pow((Y3 - Y2), 2);
	maxDist = max (max(d12, d13), d23);
	minDist = min (min(d12, d13), d23);
	double ratio = (double) maxDist / minDist;
	if (ratio < 2.0) {
		// Case in which B is missing: Max is CD, Min is AD

	} else if (ratio > 4.0) {
		// Case in which A is missing: Max is BD, Min is BC
		if (maxDist == d12) {
			if (minDist == d13) {
				B.x = X1; B.y = Y1; D.x = X2; D.y = Y2; 
			} else { D.x = X1; D.y = Y1; B.x = X2; B.y = Y2; }
		} else if (maxDist == d13) {
			if (minDist == d12) {
				B.x = X1; B.y = Y1; D.x = X3; D.y = Y3;
			} else { D.x = X1; D.y = Y1; B.x = X3; B.y = Y3; }
		} else if (maxDist == d23) {
			if (minDist == d13) {
				B.x = X3; B.y = Y3; D.x = X2; D.y = Y2; 
			} else { D.x = X3; D.y = Y3; B.x = X2; B.y = Y2; }
		}
		center.x = (B.x + D.x) / 2; 
		center.y = (B.y + D.y) / 2;
		orientation = findOrientation(B, D);
		setPosition();
	} else if (ratio < 3.2) {
		// Case in which D is missing: Max is AC, Min is BC

	} else {
		// Case in which C is missing: Max is BD, Min is AC
		if (maxDist == d12) {
			if (minDist == d13) {
				B.x = X1; B.y = Y1; D.x = X2; D.y = Y2; 
			} else { D.x = X1; D.y = Y1; B.x = X2; B.y = Y2; }
		} else if (maxDist == d13) {
			if (minDist == d12) {
				B.x = X1; B.y = Y1; D.x = X3; D.y = Y3; 
			} else { D.x = X1; D.y = Y1; B.x = X3; B.y = Y3; }
		} else if (maxDist == d23) {
			if (minDist == d13) {
				B.x = X3; B.y = Y3; D.x = X2; D.y = Y2; 
			} else { D.x = X3; D.y = Y3; B.x = X2; B.y = Y2; }
		}
		center.x = (B.x + D.x) / 2; 
		center.y = (B.y + D.y) / 2;
		orientation = findOrientation(B, D);
		setPosition();
	}
}


/* Determines orientation of the puck given two input positions. In ideal case, 
* values should be inputted in order (B, D) but one can be substituted with
* the center position if either B or D not given */
double findOrientation(struct Position p1, struct Position p2) {
	//printf("Bx: %d, By: %d, Dx: %d, Dy: %d\n", B.x, B.y, D.x, D.y);
	return atan2((p1.x - p2.x),(p1.y - p2.y));
}

/* Sets position of the robot in coordinates relative to the global frame */
void setPosition() {
	bot.x = (center.x * cos(orientation) - center.y * sin(orientation));
	bot.y = (center.x * sin(orientation) + center.y * cos(orientation));
}
