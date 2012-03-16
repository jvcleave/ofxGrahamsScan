/*
 *  ofxGrahamsScan.cpp
 *
 *  Created by jason van cleave on 3/16/12.
 *  Modified from http://www.chrisharrison.net/projects/convexHull/index.html
 *	Original Code http://www.chrisharrison.net/projects/convexHull/grahams.cpp
 *
 */

#include "ofxGrahamsScan.h"

ofxGrahamsScan::ofxGrahamsScan()
{
	firstPoint=NULL;
}

vector<ofPoint> ofxGrahamsScan::solve(vector<ofPoint> points)
{
	int minPoint=0;
	LinkedPoint tempPoints[points.size()];
	LinkedPoint *tempPtr;
	
	
	for(int i=0; i<points.size(); i++)
	{
		tempPoints[i].x = points[i].x;
		tempPoints[i].y = points[i].y;
	}
	
	//FIND MIN POINT
	for (int i=1; i<points.size(); i++)
	{
		if (tempPoints[i].y<tempPoints[minPoint].y)
		{
			minPoint=i;
		}
	}
	//SORT RANDOM POINTS
    for (int i=0; i<points.size(); i++) 
    {
        tempPoints[i].angle=findAngle(tempPoints[minPoint].x, tempPoints[minPoint].y, tempPoints[i].x, tempPoints[i].y);
        addPoint(tempPoints[i]);
    }
	
	tempPtr=firstPoint;
	
    do
	{
		//FIND LAST NODE IN LINKED LIST
        tempPtr=tempPtr->next;
		
    } while (tempPtr->next!=NULL);
	
    tempPtr->next=firstPoint; //COMPLETE CIRCULAR LINKED LIST
    firstPoint->prev=tempPtr; //COMPLETE CIRCULAR LINKED LIST
    
    tempPtr=firstPoint->next;

	scan(firstPoint->next);
	
	tempPtr=firstPoint;
    
    for (int i=0;i<points.size(); i++) //DRAW LINES FROM ONE POINT TO THE NEXT, FOR ALL NODES, BACK TO THE FIRSTPOINT
    {
		solvedPoints.push_back(ofPoint(tempPtr->x, tempPtr->y));
		solvedPoints.push_back(ofPoint(tempPtr->next->x, tempPtr->next->y));
        tempPtr=tempPtr->next; 
    }
	return solvedPoints;
}

double ofxGrahamsScan::findAngle(double x1, double y1, double x2, double y2)
{
    double deltaX=(double)(x2-x1);
    double deltaY=(double)(y2-y1);
    double angle;
	
    if (deltaX==0 && deltaY==0)
	{
		return 0;
	}
	
    angle=atan2(deltaY,deltaX)*57.295779513082;
	
	if (angle < 0)
	{
		angle += 360.;
	}
	
    return angle;
}

bool ofxGrahamsScan::isConvexPoint(LinkedPoint* P)
{
    double CWAngle=findAngle(P->x,P->y,P->prev->x,P->prev->y); //COMPUTE CLOCKWISE ANGLE
    double CCWAngle=findAngle(P->x,P->y,P->next->x,P->next->y); //COMPUTE COUNTERCLOCKWISE ANGLE
    double difAngle;
    
    
    if (CWAngle>CCWAngle)
    {
        difAngle=CWAngle-CCWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
        if (difAngle>180)
		{
			//POINT IS CONCAVE
			return false; 
		}
        else
		{
			//POINT IS CONVEX
			return true; 
		}
    }
    else if (CWAngle<CCWAngle)
    {
        difAngle=CCWAngle-CWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
		if (difAngle>180)
		{
			//POINT IS CONCAVE
			return true; 
		}
        else
		{
			//POINT IS CONVEX
			return false; 
		}
    }
    else if (CWAngle == CCWAngle)
	{
		//POINT IS COLINEAR
		return false; 
	}
}



void ofxGrahamsScan::scan(LinkedPoint *linkedPoint)
{
    LinkedPoint *tempPrev, *tempNext;
    
    if (linkedPoint==firstPoint) //IF RETURNED TO FIRST POINT, DONE
        return;
    
    if (!isConvexPoint(linkedPoint)) //IF POINT IS CONCAVE, ELIMINATE FROM PERIMETER
    {
        tempPrev=linkedPoint->prev; 
        tempNext=linkedPoint->next;
        tempPrev->next=tempNext;
        tempNext->prev=tempPrev;
		// drawLine(tempPrev, tempNext,3); //DRAW LINE SHOWING NEW EDGE
        delete linkedPoint; //FREE MEMORY
        scan(tempPrev); //RUN GRAHAM'S SCAN ON PREVIOUS POINT TO CHECK IF CONVEXITY HAS CHANGED IT
		
    } 
	else //POINT IS CONVEX
	{
		scan(linkedPoint->next); //PROCEED TO NEXT POINT
	}
}

void ofxGrahamsScan::addPoint(LinkedPoint linkedPoint)
{
    LinkedPoint *tempPoint, *tempPointA, *tempPointB, *curPoint;
    
    //ALLOCATE A NEW POINT STRUCTURE AND INITIALIZE INTERNAL VARIABLES
    tempPoint = new LinkedPoint();
    tempPoint->x=linkedPoint.x;
    tempPoint->y=linkedPoint.y;  
    tempPoint->angle=linkedPoint.angle;  
    tempPoint->next=NULL;
    tempPoint->prev=NULL;
    
    //TEST IF LIST IS EMPTY
    if (firstPoint==NULL) 
    {
        firstPoint=tempPoint;
        return;
    }
	
	//TEST IF ONLY ONE NODE IN LIST AND CURRENT NODE HAS GREATER ANGLE
    if (firstPoint->next==NULL && tempPoint->angle >= firstPoint->angle) 
    {
        firstPoint->next=tempPoint;
        tempPoint->prev=firstPoint;
        return;
    }
    
    curPoint=firstPoint;
    
	//CONTINUE THROUGH LIST UNTIL A NODE IS FOUND WITH A GREATER ANGLE THAN CURRENT NODE
    while (tempPoint->angle >= curPoint->angle && curPoint->next!=NULL)
	{
		curPoint=curPoint->next;
	}
	
	//TEST IF NODE IS FIRSTPOINT.  IF SO, ADD AT FRONT OF LIST.
    if (curPoint==firstPoint) 
    {
        firstPoint->prev=tempPoint;
        tempPoint->next=firstPoint;
        firstPoint=tempPoint;
        return;
    }
    else if (curPoint->next==NULL && tempPoint->angle >= curPoint->angle) 
    {
		//TEST IF WHILE LOOP REACHED FINAL NODE IN LIST.  IF SO, ADD AT END OF THE LIST.
        curPoint->next=tempPoint;
        tempPoint->prev=curPoint;
        return;
    }
    else  
    {
		//OTHERWISE, INTERMEDIATE NODE HAS BEEN FOUND.  INSERT INTO LIST. 
        tempPointA=curPoint->prev;
        tempPointB=curPoint->prev->next;
        tempPoint->next=tempPointB;
        tempPoint->prev=tempPointA;
        tempPoint->prev->next=tempPoint;
        tempPoint->next->prev=tempPoint;
    }
      
}








