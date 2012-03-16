/*
 *  ofxGrahamsScan.h
 *
 *  Created by jason van cleave on 3/16/12.
 *  Modified from http://www.chrisharrison.net/projects/convexHull/index.html
 *	Original Code http://www.chrisharrison.net/projects/convexHull/grahams.cpp
 *
 */

#include "ofMain.h"

//--------------------POINT DATA STRUCTURE---------------------------
struct LinkedPoint
{
    double x; //X POSITION
    double y; //Y POSITION
    LinkedPoint *next; //POINTER TO NEXT NODE IN THE LIST
    LinkedPoint *prev; //POINTER TO PREVIOUS NODE IN THE LIST
    double angle; //INTERMEDIATE ANGLE VALUE STORAGE
};



class ofxGrahamsScan
{
public:
	ofxGrahamsScan();
	vector<ofPoint> solve(vector<ofPoint> points);
	vector<ofPoint> solvedPoints;
	LinkedPoint *firstPoint;

	void addPoint(LinkedPoint linkedPoint);
	void scan(LinkedPoint *linkedPoint);
	bool isConvexPoint(LinkedPoint* linkedPoint);
	double findAngle(double x1, double y1, double x2, double y2);
	
};
