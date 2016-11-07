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
double distance;
double orientation;

void set4Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
double findOrientation();
void setPosition();

int main() {
	set4Pts(492, 330, 473, 368, 567, 367, 513, 402);
	orientation = findOrientation();
	setPosition();
	printf("x: %d, y: %d\n", bot.x, bot.y);
}


// Sets the center of the field in the local frame of the robot
// Defines the robot's distance (unscaled) from the center of the frame
void set4Pts(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4) {
	int minDist, maxDist, d12, d13, d14, d23, d24, d34;
	d12 = (X2-X1)^2 + (Y2 - Y1)^2;
	d13 = (X3-X1)^2 + (Y3 - Y1)^2;
	d14 = (X4-X1)^2 + (Y4 - Y1)^2;
	d23 = (X3-X2)^2 + (Y3 - Y2)^2;
	d24 = (X4-X2)^2 + (Y4 - Y2)^2;
	d34 = (X4-X2)^2 + (Y4 - Y2)^2;
	maxDist = max (max (max(d12, d13), max(d14, d23)), max(d24, d34));
	minDist = min (min (min(d12, d13), min(d14, d23)), min(d24, d34));

	// check largest distance values to determine B and D, find center
	if (maxDist == d12) { 
		if (minDist == d13 || minDist == d14) {
			B.x = X1; B.y = Y1; D.x = X2; D.y = Y2;
		} else { D.x = X1; D.y = Y1; B.x = X2; B.y = Y2; }
	} else if (maxDist == d13) { 
		if (minDist == d13 || minDist == d14) {
			B.x = X1; B.y = Y1; D.x = X3; D.y = Y3;
		} else { D.x = X1; D.y = Y1; B.x = X3; B.y = Y3; }
	} else if (maxDist == d14) {
		if (minDist == d13 || minDist == d14) {
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
		printf("Something is wrong. Largest distance is %d\n", maxDist);
	}

	center.x = (B.x + D.x) / 2;
	center.y = (B.y + D.y) / 2;
	distance = sqrt(center.x^2 + center.y^2);
}

/* Determines orientation of the puck given two input positions. In ideal case, 
* values should be inputted in order (B, D) but one can be substituted with
* the center position if either B or D not given */
double findOrientation(struct Position p1, struct Position p2) {
	return atan2((p1.x - p2.x),(p1.y - p2.y));
}

/* Sets position of the robot in coordinates relative to the global frame */
void setPosition() {
	bot.x = -(center.x * cos(orientation) - center.y * sin(orientation));
	bot.y = -(center.x * sin(orientation) + center.y * cos(orientation));
}
