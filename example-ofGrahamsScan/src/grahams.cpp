 /*
 * Chris' implementation of Graham's Scan
 * Copyright (C) 2003 Chris Harrison
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include <time.h>

//--------------------POINT DATA STRUCTURE---------------------------
struct point
{
    double x; //X POSITION
    double y; //Y POSITION
    point *next; //POINTER TO NEXT NODE IN THE LIST
    point *prev; //POINTER TO PREVIOUS NODE IN THE LIST
    double angle; //INTERMEDIATE ANGLE VALUE STORAGE
};


//--------------------GLOBAL VARIABLES---------------------------
const int NumPoints = 15; // n<1000
SDL_Surface *screen; //GLOBAL SDL GRAPHICS SURFACE POINTER
point* firstPoint; //GLOBAL POINTER TO MIN POINT IN DOUBLELY LINKED LIST


//--------------------GRAHAM'S SCAN FUNCTIONS---------------------------
void grahamInit(); //INITIALIZE VARIABLES, RANDOMLY GENERATE POINTS,
                   //LOCATE MIN POINT, AND SORT POINTS BY RELATIVE ANGLES
void grahamMain(); //SETUP, RUN GRAHAM'S SCAN, AND DISPLAY RESULTS
void grahamScan(point *P); //ACTUAL GRAHAM'S SCAN PROCEDURE
bool isConvexPoint(point *P); //TEST POINT FOR CONVEXITY
void addPoint(point Point); //ADDS POINT TO DOUBLELY LINKED LIST (USED DURING SORTING)
double findAngle(double x1, double y1, double x2, double y2); //FIND ANGLE GIVEN TWO POINTS


//--------------------AUXILARY GRAPHICS FUNCTIONS---------------------------
void initScreen(); //SETUP THE GRAPHICS SURFACE AND WINDOW
void drawPoints(); //DRAW POINTS FROM GLOBAL DOUBLELY LINKED LIST
void drawLine(point *A, point *B, int color); //DRAWS A LINE WITH 3 COLOR POSSIBILITIES
void drawPermeter(int color); //DRAWS PERIMETER WITH 3 COLOR POSSIBILITIES
void graphicsLoop(); //MAIN GRAPHICS LOOP
void printPoints(); //PRINTS ALL POINTS IN DOUBLELY LINKED LIST


//--------------------MAIN---------------------------
int main(int argc, char *argv[])
{
    initScreen(); //INITIALIZE THE GRAPHICS WINDOW
    srand(time(NULL)); //SEED THE RANDOM NUMBER GENERATER WITH THE TIME
    grahamMain(); //RUN ENTIRE GRAHAM'S SCAN PROCEDURE
    graphicsLoop(); //LOOP UNTIL USER QUIT
    return 0; //EXIT
}


void grahamMain()
{
    grahamInit(); //INITIALIZE DATA FOR GRAHAM'S SCAN
    //printPoints(); //PRINT OUT SORTED POINTS
    drawPoints(); //RENDER POINTS ON SCREEN
    drawPermeter(1); //DRAW PERIMETER BEFORE GRAHAM'S SCAN
    grahamScan(firstPoint->next); //RUN GRAHAM'S SCAN STARTING AT SECOND NODE CLOCKWISE
    drawPermeter(2); //DRAW PERIMETER AFTER GRAHAM'S SCAN
    //printPoints(); //PRINT OUT CONVEX HULL
}


void grahamScan(point *P)
{
    point *tempPrev, *tempNext;
    
    if (P==firstPoint) //IF RETURNED TO FIRST POINT, DONE
        return;
    
    if (!isConvexPoint(P)) //IF POINT IS CONCAVE, ELIMINATE FROM PERIMETER
    {
        tempPrev=P->prev; 
        tempNext=P->next;
        tempPrev->next=tempNext;
        tempNext->prev=tempPrev;
        drawLine(tempPrev, tempNext,3); //DRAW LINE SHOWING NEW EDGE
        delete P; //FREE MEMORY
        grahamScan(tempPrev); //RUN GRAHAM'S SCAN ON PREVIOUS POINT TO CHECK IF CONVEXITY HAS CHANGED IT

    }
    else //POINT IS CONVEX
        grahamScan(P->next); //PROCEED TO NEXT POINT
}


void grahamInit()
{
    int minPoint=0;
    double tempAngle=0;
    point tempPoints[1000]; //CREATE STATIC ARRAY FOR RANDOM POINT GENERATION
    point *tempPtr;
    
    firstPoint=NULL; //INIT FIRSTPOINT POINTER
    
    for (int i=0;i<NumPoints;i++) //GENERATE RANDOM POINTS
    {
        tempPoints[i].x=rand()%400+50;
        tempPoints[i].y=rand()%400+50;
    }
    
    for (int k=1;k<NumPoints;k++)  //FIND MIN POINT
        if (tempPoints[k].y<tempPoints[minPoint].y)
            minPoint=k;
    
    //cout<<"MinPoint: "<<tempPoints[minPoint].x<<" "<<tempPoints[minPoint].y<<endl;  //PRINT OUT MINIMAL POINT
    
    for (int i=0;i<NumPoints;i++) //SORT RANDOM POINTS
    {
        tempPoints[i].angle=findAngle(tempPoints[minPoint].x,tempPoints[minPoint].y,tempPoints[i].x,tempPoints[i].y);
        addPoint(tempPoints[i]);
        //cout<<"angle: "<<tempPoints[i].angle<<" x: "<<tempPoints[i].x<<" y: "<<tempPoints[i].y<<endl;
    }
    
    tempPtr=firstPoint;
    do  //FIND LAST NODE IN LINKED LIST
    {
        tempPtr=tempPtr->next;
    } while (tempPtr->next!=NULL);
        
    tempPtr->next=firstPoint; //COMPLETE CIRCULAR LINKED LIST
    firstPoint->prev=tempPtr; //COMPLETE CIRCULAR LINKED LIST
    
    tempPtr=firstPoint->next;
    for (int i=1;i<NumPoints;i++) //DRAW LINES FROM MIN POINT TO ALL OTHERS
    {
        drawLine(firstPoint, tempPtr, 3);
        tempPtr=tempPtr->next;
    } 
}

bool isConvexPoint(point* P)
{
    double CWAngle=findAngle(P->x,P->y,P->prev->x,P->prev->y); //COMPUTE CLOCKWISE ANGLE
    double CCWAngle=findAngle(P->x,P->y,P->next->x,P->next->y); //COMPUTER COUNTERCLOCKWISE ANGLE
    double difAngle;
    
    drawLine(P->prev, P->next, 3);
    
    if (CWAngle>CCWAngle)
    {
        difAngle=CWAngle-CCWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
        if (difAngle>180)
            return false; //POINT IS CONCAVE
        else
            return true; //POINT IS CONVEX
    }
    else if (CWAngle<CCWAngle)
    {
        difAngle=CCWAngle-CWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
        if (difAngle>180)
            return true; //POINT IS CONVEX
        else
            return false; //POINT IS CONCAVE
    }
    else if (CWAngle == CCWAngle)
	return false; //POINT IS COLINEAR
}


void addPoint(point Point)
{
    point *tempPoint,*tempPointA,*tempPointB, *curPoint;
    
    //ALLOCATE A NEW POINT STRUCTURE AND INITIALIZE INTERNAL VARIABLES
    tempPoint = new point;
    tempPoint->x=Point.x;
    tempPoint->y=Point.y;  
    tempPoint->angle=Point.angle;  
    tempPoint->next=NULL;
    tempPoint->prev=NULL;
    
    
    if (firstPoint==NULL) //TEST IF LIST IS EMPTY
    {
        firstPoint=tempPoint;
        return;
    }

    if (firstPoint->next==NULL && tempPoint->angle >= firstPoint->angle) 
    //TEST IF ONLY ONE NODE IN LIST AND CURRENT NODE HAS GREATER ANGLE
    {
        firstPoint->next=tempPoint;
        tempPoint->prev=firstPoint;
        return;
    }
    
    curPoint=firstPoint;
    
    while (tempPoint->angle >= curPoint->angle && curPoint->next!=NULL)
    //CONTINUE THROUGH LIST UNTIL A NODE IS FOUND WITH A GREATER ANGLE THAN CURRENT NODE
        curPoint=curPoint->next;
  
    if (curPoint==firstPoint) //TEST IF NODE IS FIRSTPOINT.  IF SO, ADD AT FRONT OF LIST.
    {
        firstPoint->prev=tempPoint;
        tempPoint->next=firstPoint;
        firstPoint=tempPoint;
        return;
    }
    else if (curPoint->next==NULL && tempPoint->angle >= curPoint->angle) 
    //TEST IF WHILE LOOP REACHED FINAL NODE IN LIST.  IF SO, ADD AT END OF THE LIST.
    {
        curPoint->next=tempPoint;
        tempPoint->prev=curPoint;
        return;
    }
    else //OTHERWISE, INTERMEDIATE NODE HAS BEEN FOUND.  INSERT INTO LIST.  
    {
        tempPointA=curPoint->prev;
        tempPointB=curPoint->prev->next;
        tempPoint->next=tempPointB;
        tempPoint->prev=tempPointA;
        tempPoint->prev->next=tempPoint;
        tempPoint->next->prev=tempPoint;
    }
    
    return;   
}

double findAngle(double x1, double y1, double x2, double y2)
{
    double deltaX=(double)(x2-x1);
    double deltaY=(double)(y2-y1);
    double angle;
 
    if (deltaX==0 && deltaY==0)
        return 0;
   
    angle=atan2(deltaY,deltaX)*57.295779513082;
  
   if (angle < 0)
 	angle += 360.;

    return angle;
}


//--------------------------PERIPHERAL FUNCTIONS-------------------------------

void drawPermeter(int color)
{
    point *tempPtr=firstPoint;
    
    for (int i=0;i<NumPoints;i++) //DRAW LINES FROM ONE POINT TO THE NEXT, FOR ALL NODES, BACK TO THE FIRSTPOINT
    {
        drawLine(tempPtr, tempPtr->next, color);  //DRAW LINE
        tempPtr=tempPtr->next; 
    }   
}


void printPoints()
{
    point *curPoint=firstPoint;
    
    do
    {
        cout<<"angle: "<<curPoint->angle<<" x: "<<curPoint->x<<" y: "<<curPoint->y<<endl;
        curPoint=curPoint->next;
    } while (curPoint!=firstPoint); //CONTINUE UNTIL HAVING LOOPED BACK AROUND TO FIRSTPOINT
}



void drawPoints()
{
    point *curPoint = firstPoint;
    
    do 
    {
        filledCircleRGBA (screen, curPoint->x, curPoint->y, 3, 255, 0, 0, 255); //DRAW RED CIRCLE OF RADIUS 3
        curPoint=curPoint->next;
    } while (curPoint!=firstPoint); //CONTINUE UNTIL HAVING LOOPED BACK AROUND TO FIRSTPOINT
    
    return;
}


void graphicsLoop()
{
    SDL_Event event;
    Uint8 *keys;
    
    SDL_Flip(screen); //FLIP GRAPHICS BUFFER
    
    while (true)
    {
        SDL_PollEvent(&event);
        keys=SDL_GetKeyState(NULL);
        
        if (keys[SDLK_ESCAPE]) //TEST FOR USER INPUT (ESCAPE KEY)
        {
            exit(1); //EXIT PROGRAM
        }
        
        if(keys[SDLK_SPACE]) //TEST FOR USER INPUT (SPACE BAR)
        {
            boxRGBA(screen, 0, 0, 500, 500, 0, 0, 0, 255); //CLEAR SCREEN WITH BLACK BOX
            grahamMain(); //GENERATE BRAND NEW CONVEX HULL
            SDL_Flip(screen); //FLIP GRAPHICS BUFFER
        }
        
        //SDL_Delay(100);
    }        
}


void initScreen()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) //TEST SDL STARTED PROPERLY
        exit(1); //ON ERROR, EXIT

    atexit(SDL_Quit); //CLEAN UP SDL BEFORE EXITING
    screen=SDL_SetVideoMode(500, 500, 16, SDL_SWSURFACE); //SET SCREEN MODE
}


void drawLine(point *A, point *B, int color)
{
    if (color==1) //IF COLOR==1, DRAW RED LINE 
        lineRGBA (screen, A->x, A->y, B->x, B->y, 225, 0, 0, 255);
    if (color==2) //IF COLOR==1, DRAW GREEN LINE
        lineRGBA (screen, A->x, A->y, B->x, B->y, 0, 255, 0, 255);
    if (color==3) //IF COLOR==1, DRAW LIGHT BLUE LINE
        lineRGBA (screen, A->x, A->y, B->x, B->y, 0, 150, 220, 255);
}

