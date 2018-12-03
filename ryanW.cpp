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
#include <cmath>
#include <ctime>
using namespace std;

typedef float Flt;
typedef float Vec[3];

#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))

const int MAX_BULLETS = 30;

extern X11_wrapper x11;
extern void displayTowers();

//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//+++++++++++++END OF STANDARD ITEMS++++++++++++++//

//+++++++++++++START OF CREDIT DISPLAY++++++++++++++//
void ryanW(Rect x, int y) 
{
    ggprint8b(&x, y, 0x00ff0000, "Ryan Wallace: Towers & Bullets");
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

///+++++++++++++START OF TOWER CLASS ITEMS++++++++++++++//
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
const int MAX_TOWERS = 5;
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
	p->pos[0] = x;
	p->pos[1] = y;
	//create tower image
	glGenTextures(1, &p->tex);
	glBindTexture(GL_TEXTURE_2D, p->tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		     GL_RGB, GL_UNSIGNED_BYTE, towerList[0].data);

numTowers++;
}

//+++++++++++++DISPLAY TOWER TO SCREEN++++++++++++++//
void displayTowers() 
{
    if (numTowers < 1)
        return;
    Tower *p;
    for (int i = 0; i < numTowers; i++) {
        p = &basicTower[i];
	    static int wid = 30;
	    glColor3ub(255,255,255);
            glPushMatrix();
	    glBindTexture(GL_TEXTURE_2D, p->tex);
	    glTranslated(p->pos[0], p->pos[1], 0);
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

//+++++++++++++END OF TOWER CLASS ITEMS++++++++++++++//

//+++++++++++++START OF BULLET CLASS ITEMS+++++++++++++++++//
class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};

//++++++++++++++BULLET ATTRIBUTES+++++++++++++++++//
static int numBullets = 0;
const int MAX_BULLETS = 11;
Barr = new Bullet[MAX_BULLETS];

//+++++++++++++START OF BULLET PHYSICS++++++++++++++//
//Must be in physics() function call
void bulletPhysics()
{
	Flt d0,d1,dist;
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.numbullets) {
		Bullet *b = &g.barr[i];
		//Destroy bullets after 1.5 seconds
		double ts = timeDiff(&b->time, &bt);
		if (ts > 1.5) {
			//delete the bullet
			memcpy(&g.barr[i], &g.barr[g.numbullets-1],
				sizeof(Bullet));
			g.numbullets--;
			//do not increment i
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)gl.xres;
		}
		else if (b->pos[0] > (float)gl.xres) {
			b->pos[0] -= (float)gl.xres;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)gl.yres;
		}
		else if (b->pos[1] > (float)gl.yres) {
			b->pos[1] -= (float)gl.yres;
		}
		i++;
	}
	
	if (gl.keys[XK_space]) {
		//.5 seconds between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (ts > 0.5) {
			timeCopy(&g.bulletTimer, &bt);
			if (g.numbullets < MAX_BULLETS) {
				//shoot a bullet
				
				//check how to get a tower item in this function
				Tower *p;
				//^^^^^^^^^^^^^^^^^
				
				Bullet *b = &g.barr[g.nbullets];
				timeCopy(&b->time, &bt);
				
				//check if p->pos is valid
				b->pos[0] = p->pos[0];
				b->pos[1] = p->pos[1];
				//^^^^^^^^^^^^^^^^^
				
				//force a bullet direction to 90 degrees
				Flt rad = (90.0 / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				g.numbullets++;
			}
		}
	}
}

//+++++++++++++START OF BULLET RENDER++++++++++++++//
//must be in render() function call
void bulletRender() {
	Bullet *b = &g.barr[0];
	for (int i=0; i<g.numbullets; i++) {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
			glVertex2f(b->pos[0],      b->pos[1]);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]);
			glVertex2f(b->pos[0],      b->pos[1]-1.0f);
			glVertex2f(b->pos[0],      b->pos[1]+1.0f);
			glColor3f(0.8, 0.8, 0.8);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
		++b;
	}
}

//+++++++++++++++END OF BULLET CLASS ITEMS++++++++++//


