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
#include "RyanW.h"
using namespace std;

typedef float Flt;
typedef float Vec[3];

#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))

extern X11_wrapper x11;
extern void init_animatedTower(Tower* p);
extern void show_animatedTower(float x, float y, GLuint texid);

const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

//+++++++++++++TOWER ATTRIBUTES++++++++++++++//
const int MAX_TOWERS = 5;
static int numTowers = 0;
Image towerList[1] = {"./images/cannonanimated1kms.gif"};

//++++++++++++++BULLET ATTRIBUTES+++++++++++++++++//
static int numBullets = 0;
struct timespec bulletTimer;

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

//+++++++++++++START OF BULLET CLASS ITEMS+++++++++++++++++//
/*DEFINED IN HEADER FILE
class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};

///+++++++++++++START OF TOWER CLASS ITEMS++++++++++++++//
class Tower {
public:
	int onoff;
	int frame;
	Vec pos;
	Image *image;
	Bullet *barr;
	GLuint tex;
	struct timespec time;
	Tower() {
		barr = new Bullet[MAX_BULLETS];
		onoff = 0;
		frame = 0;
		image = NULL;
	}
};
*/
static Tower basicTower[MAX_TOWERS];

//+++++++++++++TOWER CREATION++++++++++++++//
void createTower(int x, int y)
{	
	Tower *p; 
	p = &basicTower[numTowers];
	init_animatedTower(p);
	int w, h;
	w = towerList[0].width;
	h = towerList[0].height;
	p->pos[0] = x;
	p->pos[1] = y;
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
	    float x = p->pos[0];
	    float y = p->pos[1];
	    show_animatedTower(x, y, p->tex);
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

//+++++++++++++START OF BULLET PHYSICS++++++++++++++//
//Must be in physics() function call
void bulletPhysics(int x, int y)
{
	//Flt d0,d1,dist;
	/*
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	for (int i = 0; i < MAX_TOWERS; i++) {
		for (int j = 0; j < numBullets; j++) {
			Bullet *b = basicTower[i].barr[j];
			//Destroy bullets after 1.5 seconds
			double ts = timeDiff(b->time, &bt);
			if (ts > 1.5) {
				memcpy(basicTower[i].barr[j], basicTower[i].barr[numBullets-1],
					sizeof(Bullet));
				numBullets--;
				continue;
			}
			*/
	for (int i = 0; i < MAX_TOWERS; i++) {
		Bullet *b;
		b = basicTower[i].barr;
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)x;
		}
		else if (b->pos[0] > (float)x) {
			b->pos[0] -= (float)x;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)y;
		}
		else if (b->pos[1] > (float)y) {
			b->pos[1] -= (float)y;
		}
	}
}
void shootBullets() {
	//.5 seconds between each bullet
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&bulletTimer, &bt);
	if (ts > 0.5) {
		timeCopy(&bulletTimer, &bt);
		if (numBullets < MAX_BULLETS) {			
			//Tower *p;
			for (int i = 0; i < MAX_TOWERS; i++) {
				//Tower *p;
				Bullet *b;
				b = basicTower[i].barr;
				//timeCopy(b->time, &bt);
				/*
				//check if p->pos is valid
				b->pos[0] = p->pos[0];
				b->pos[1] = p->pos[1];
				*/
				//force a bullet direction to 90 degrees
				Flt rad = (90.0 / 360.0f) * 3.14 * 2.0;
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				b++;
			}
		}
	}
}

//+++++++++++++START OF BULLET RENDER++++++++++++++//
//must be in render() function call
void bulletRender() {
	for (int i = 0; i < MAX_TOWERS; i++) {
		Bullet *b;
		b = basicTower[i].barr;
		for (int j = 0; j < numBullets; j++) {
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
}

//+++++++++++++++END OF BULLET CLASS ITEMS++++++++++//


