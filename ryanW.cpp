//Author: Ryan Wallace
//
//Prints out the names of one member
//

// Start of all standardized items
#include "fonts.h"
#include <iostream>
#include <GL/glx.h>
#include <math.h>
using namespace std; 

Image towerList[1] = {"./images/ob.jpg"};

// End of all standardized items

void ryanW(Rect x, int y) 
{
    ggprint8b(&x, y, 0x00ff0000, "Ryan Wallace");
}

void showRyanPicture(int x, int y, GLuint texid)
{
    static float angle = 0.0f; 
    float fx = (float)x;
    float fy = (float)y;
    fx += sin(angle) * 10.0f;
    angle += 0.2f;
    glPushMatrix();
    glColor3ub(255, 255, 255);
    int wid=40;
    glTranslatef(fx, fy, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
     glEnd();
     glPopMatrix();
}

// Tower Object creation

class Tower {
public:
	int onoff;
	int frame;
	Vec pos;
	Image *image;
	GLuint tex;
	struct timespec time;
	Tower() {
		onoff = 0;
		frame = 0;
		image = NULL;
	}
};

// Tower Items
const int MAX_TOWERS = 10;
static int numTowers = 0;
static Tower basicTower[MAX_TOWERS];

class level {
public:
	Vec spawn[1];
	level() {
		spawn = {0};
	}
}level;

void setSpawn(Flt x, Flt y, Flt z) 
{
	level.spawn = {{x}, {y}, {z}};
};


// Basic Tower Creation
void createTower(int i, int x, int y)
{	
	Tower *p; 
	p = &basicTower[numTowers];
	int w, h;
	w = towerList[i].width;
	h = towerList[i].height;
	//setup image for the unit.
	glGenTextures(1, &p->tex);
	glBindTexture(GL_TEXTURE_2D, p->tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		         GL_RGB, GL_UNSIGNED_BYTE, towerList[i].data);

numTowers++;

// Displays Towers onto the screen
void displayTowers() {
    if (numTowers < 1)
        return;
    Tower *p;
    for (int i = 0; i < numTowers; i++) {
        p = &basicTower[i];
        float x = p->pos[0];
        float y = p->pos[1];
	    static int wid = 30;
	    glColor3ub(255,255,255);
        glPushMatrix();
	    
	    glBindTexture(GL_TEXTURE_2D, p->tex);
	    glTranslated(x, y, 0);
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
        glEnd();
        
	    glPopMatrix();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}












