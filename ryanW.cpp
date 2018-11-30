//Author: Ryan Wallace
//updated: NOV 27 2018
//
//contains code for Ryan's Credit Display
//This file holds the code for all tower items and anything that has to do with tower interactions and placement. 
//

//+++++++++++++START OF STANDARD ITEMS++++++++++++++//
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
#include "image.h"
#include <iostream>
using namespace std;

typedef float Flt;
typedef float Vec[3];

const int MAX_BULLETS = 30;


Image TowerList[1] = {"./images/ob.jpg"};
extern X11_wrapper x11;
extern void displayTowers(float x, float y, GLuint texid);
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//+++++++++++++END OF STANDARD ITEMS++++++++++++++//

//+++++++++++++START OF CREDIT DISPLAY++++++++++++++//
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
//+++++++++++++END OF CREDIT DISPLAY++++++++++++++//

///+++++++++++++START OF TOWER CLASS++++++++++++++//
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

//+++++++++++++TOWER ATTRIBUTES++++++++++++++//
const int MAX_TOWERS = 4;
static int numTowers = 0;
static Tower basicTower[MAX_TOWERS];
Image towerList[1] = {"./images/cannonanimated1kms.gif"};

//+++++++++++++TOWER CREATION++++++++++++++//
void createTower(int x, int y)
{	
	Tower *p; 
	p = &basicTower[numTowers];
	int w, h;
	w = towerList[0].width;
	h = towerList[0].height;
	p.pos[0] = x;
	p.pos[1] = y;
	//create tower image
	glGenTextures(1, &p->tex);
	glBindTexture(GL_TEXTURE_2D, p->tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		     GL_RGB, GL_UNSIGNED_BYTE, towerList[i].data);

numTowers++;
}

//+++++++++++++TOWER DISPLAY TO SCREEN++++++++++++++//
void displayTowers() {
    if (numTowers < 1)
        return;
    Tower *p;
    for (int i = 0; i < numTowers; i++) {
        p = &basicTower[i];
        //float x = p->pos[0];
        //float y = p->pos[1];
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

//+++++++++++++BULLET CLASS CREATION++++++++++++++//
class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};


//+++++++++++++END OF TOWER CLASS ITEMS++++++++++++++//

//+++++++++++++START OF CODE SANDBOX++++++++++++++//
/*
Barr = new Bullet[MAX_BULLETS];
numBullets = 0;

///////start of tower shooting code////////
struct timespec bt;
clock_gettime(CLOCK_REALTIME, &bt);
double ts = timeDiff(&g.bulletTimer, &bt);
if (ts > 0.3) {
	timeCopy(&g.bulletTimer, &bt);
	//shoot bullets
	if (g.numBullets < MAX_BULLETS) {
		Bullet *b = &g.barr[g.numBullets];
		timeCopy(&b->time, &bt);
		b->pos[0] = tower.pos[0];
		b->pos[1] = tower.pos[1];
		b->pos[0] += xdir*20.0f;
		b->pos[1] += ydir*20.0f;
		b->vel[0] += xdir*6.0f + rnd()*0.1;
		b->vel[1] += ydir*6.0f + rnd()*0.1;
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
		++g.numBullets;
	}
}
////////end of tower shooting code/////////

*/
//+++++++++++++END OF CODE SANDBOX++++++++++++++//




