//Author: Jonathan Crawford
/*
This file contains the code for slimes and pathing information.
Slime: creat, delet, move, display and reset
path: follow a path, set a path, custom pathing.
*/
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
#include "image.h"
#include <iostream>
#include "jonathanC.h"
using namespace std;

typedef float Flt;
typedef float Vec[3];

struct Point {
	float x, y, dist;
};

extern X11_wrapper x11;
extern void show_animatedUnit(float x, float y, GLuint texid);
extern void init_animatedUnit(Unit* p);

/*
class Path {
public:
	int npathPoints;
	Vec pathPoints[15];
	int pathID;
	Path(int p) {
		pathID = p;
		initPaths(pathID);
	}
	
};

class Unit {
public:
	int checkpoint, health, moveSpeed, pathPoints;
	Vec pos;
	Vec vel;
	Image *image;
	Path *path;
	GLuint tex;
	int frame, size;
	struct timespec time;
	Unit() {
		path = NULL;
		pathPoints = 1;
		image = NULL;
		checkpoint = 1;
		moveSpeed = 1;
		health = 100;
		frame = 0;
		size = 1;
	}
};
*/

Path waterway(1), bridge(2), lakeview(3);
//----vector functions----
void vecCopy(Vec a, Vec b);
void vecSub(Vec a, Vec b, Vec c);
void vecNormalize(Vec a);
float getDistance(Vec a, Vec b);

//----------------------------------slime stuff-----
const int MAX_SLIME = 40;
static int nslime = -1;
static Unit slime[MAX_SLIME];
static int speed = 1;
static Path *currentPath = &waterway;

void createSlime();
void moveSlime(int xres, int yres);
void deleteSlime(int);
void resetSlime();
void showSlime();
int getSlimeCount();
void increseSpeed();

//----------------------------------pathing stuff---
const int MAX_POINTS = 100;
static Point pt[MAX_POINTS];
static int npoint = -1;
static int customPathing = 0;

int getPointCount();
void initPaths(int ID);
void toggleCustomPathing(int onOff);
void setPath(int pathID);
float next_x(Path *path, int index);
float next_y(Path *path, int index);
int get_totalPathPoints(Path *path);
void resetPath();
void getCords(int x, int y, int yres);
void showCords();

//----------------vector functions-------------------------------------

//normalizes a given vector.
void vecNormalize(Vec a) 
{
	float dist = getDistance(a, a);
	a[0] /= dist;
	a[1] /= dist;
}

//subtracts two vectors and puts the result in a third vector.
//(a-b=c)
void vecSub(Vec a, Vec b, Vec c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

//copies a given vector into another vector(destination, source)
void vecCopy(Vec a, Vec b) 
{
	b[0] = a[0];
	b[1] = a[1];
	b[2] = a[2];
}

//finds distance between two vectors
float getDistance(Vec a, Vec b)
{
	//check for same vector.
	if (a[0] == b[0] && a[1] == b[1]) {
		return sqrt(a[0]*a[0] + a[1]*a[1]);
	} else {
		Vec v;
		vecSub(b, a, v);
		return getDistance(v, v);
	}
}

//----------------credit functions-------------------------------------

//displays the author's name (Jonathan Crawford)
void jonathanC(Rect x, int y)
{
	ggprint8b(&x, y, 0x00ff0000, "Jonathan Crawford");
}

//Display an image to the screen
void showJonathanPicture(int x, int y, GLuint textid)
{
	glColor3ub(255,255,255);
	static float angle = 0.0f;
	static int wid = 30;
	wid += sin(angle) * 10;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0;
	a += sin(angle) * 10.0f;
	fx += a;
	angle += 0.2f;
	glPushMatrix();
	glTranslatef(fx,fy,0);
	glRotatef(a, 0, 0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textid);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

	glEnd();
	glPopMatrix();
}

//----------------------------slime functions----------------------------

//speed new slimes up.
void increseSpeed()
{
	speed += 1;
}

//slow new slimes down.
void decreaseSpeed()
{	
	if (speed > 1)
		speed -= 1;
}

//adds a slime to the array of slimes. 
void createSlime()
{	
	Unit *s; 
	s = &slime[nslime];
	//init_unit(p);
	init_animatedUnit(s);

	if (customPathing) {
		if (npoint > 0) {
			s->path = NULL;
			s->pos[0] = pt[0].x;
			s->pos[1] = pt[0].y;
			s->pos[2] = 0;
			s->pathPoints = 0;
			nslime++;
		}
	} else {
		s->path = currentPath;
		s->pos[0] = next_x(currentPath, 0);
		s->pos[1] = next_y(currentPath, 0);
		s->pos[2] = 0;
		s->pathPoints = get_totalPathPoints(currentPath);
		nslime++;
	}

	s->moveSpeed = speed;
	s->checkpoint = 1;
	return;
}

//Steps through an array to display all currently generated slimes.
void showSlime() {
	if (nslime < 1)
		return;

	Unit *p;

	for (int i = 0; i < nslime; i++) {
		p = &slime[i];
		float x = p->pos[0];
		float y = p->pos[1];
		//show_unit(x,y,p->tex);
		show_animatedUnit(x,y,p->tex);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

//moves slimes along path.
void moveSlime(int xres, int yres)
{
	for (int i = 0; i < nslime; i++) {
	
		Unit *s;
		s = &slime[i];
		int sp = s->moveSpeed;

		if (s->path == NULL && 
			(s->checkpoint != npoint) && 
			(s->pos[2] < 20) &&
			npoint > 1) {

			Vec v, p = {0};

			p[0] = pt[s->checkpoint].x;
			p[1] = pt[s->checkpoint].y;
			vecSub(p, s->pos, v);
			vecNormalize(v);
			vecCopy(v, s->vel);
			s->pos[2] = getDistance(s->pos, p);
			s->checkpoint += 1;

		} else if (s->checkpoint != s->pathPoints && 
					s->pos[2] < 20 &&
					s->path != NULL) {

			Vec v, p = {0};

			p[0] = next_x(s->path, s->checkpoint);
			p[1] = next_y(s->path, s->checkpoint);
			vecSub(p, s->pos, v);
			vecNormalize(v);
			vecCopy(v, s->vel);
			s->pos[2] = getDistance(s->pos, p);
			s->checkpoint += 1;
		}


		s->pos[0] += s->vel[0]*sp;
		s->pos[1] += s->vel[1]*sp;
		s->pos[2] -= 1*sp;
	}

	for (int i = 0; i < nslime; i++) {
		Unit *s;
		s = &slime[i];
		if (s->pos[0] < 0 || s->pos[0] > xres ||
			s->pos[1] < 0 || s->pos[1] > yres) {
			deleteSlime(i);
		}
	}
}

//replaces one slime with slime at the end of array and decreases total slime
//count.
void deleteSlime(int s)
{

	if (s == (nslime-1)) {
		nslime -= 1;
		return;
	}
	slime[s] = slime[nslime-1];
	nslime -= 1;

}

//clears slime array and slime count.
void resetSlime()
{
	memset(slime, 0, sizeof(slime));
	nslime = -1;
}

//returns total number of slimes.
int getSlimeCount()
{
	return nslime;
}

//returns pointer to a specified slime
Unit* getSlime(int slimeNum)
{
	Unit *p = &slime[slimeNum];
	return p;
}

//---------------------Pathing ------------------------------------

//initializes path arrays. 1 = waterway, 2 = bridge, 3 = lakeview
void initPaths(int ID)
{	
	if (ID == 1) {

		Path *p = &waterway;
		p->npathPoints = 8;
		p->pathPoints[0][0] = 225;
		p->pathPoints[0][1] = 262;

		p->pathPoints[1][0] = 225;
		p->pathPoints[1][1] = 183;

		p->pathPoints[2][0] = 517;
		p->pathPoints[2][1] = 183;

		p->pathPoints[3][0] = 517;
		p->pathPoints[3][1] = 430;

		p->pathPoints[4][0] = 703;
		p->pathPoints[4][1] = 430;

		p->pathPoints[5][0] = 703;
		p->pathPoints[5][1] = 271;

		p->pathPoints[6][0] = 1114;
		p->pathPoints[6][1] = 271;

		p->pathPoints[7][0] = 1114;
		p->pathPoints[7][1] = 15;

	} else if (ID == 2) {

		Path *p = &bridge;
		p->npathPoints = 12;
		p->pathPoints[0][0] = 225;
		p->pathPoints[0][1] = 262;

		p->pathPoints[1][0] = 225;
		p->pathPoints[1][1] = 183;

		p->pathPoints[2][0] = 517;
		p->pathPoints[2][1] = 183;

		p->pathPoints[3][0] = 517;
		p->pathPoints[3][1] = 430;

		p->pathPoints[4][0] = 300;
		p->pathPoints[4][1] = 430;

		p->pathPoints[5][0] = 300;
		p->pathPoints[5][1] = 600;

		p->pathPoints[6][0] = 520;
		p->pathPoints[6][1] = 600;

		p->pathPoints[7][0] = 520;
		p->pathPoints[7][1] = 800;

		p->pathPoints[8][0] = 578;
		p->pathPoints[8][1] = 840;

		p->pathPoints[9][0] = 706;
		p->pathPoints[9][1] = 840;

		p->pathPoints[10][0] = 706;
		p->pathPoints[10][1] = 714;

		p->pathPoints[11][0] = 1227;
		p->pathPoints[11][1] = 714;

	} else if (ID == 3) {

		Path *p = &lakeview;
		p->npathPoints = 12;

		p->pathPoints[0][0] = 225;
		p->pathPoints[0][1] = 262;

		p->pathPoints[1][0] = 225;
		p->pathPoints[1][1] = 183;

		p->pathPoints[2][0] = 517;
		p->pathPoints[2][1] = 183;

		p->pathPoints[3][0] = 517;
		p->pathPoints[3][1] = 430;

		p->pathPoints[4][0] = 300;
		p->pathPoints[4][1] = 430;

		p->pathPoints[5][0] = 300;
		p->pathPoints[5][1] = 600;

		p->pathPoints[6][0] = 520;
		p->pathPoints[6][1] = 600;

		p->pathPoints[7][0] = 520;
		p->pathPoints[7][1] = 800;

		p->pathPoints[8][0] = 401;
		p->pathPoints[8][1] = 880;

		p->pathPoints[9][0] = 200;
		p->pathPoints[9][1] = 880;

		p->pathPoints[10][0] = 189;
		p->pathPoints[10][1] = 715;

		p->pathPoints[11][0] = 29;
		p->pathPoints[11][1] = 715;
	}	

}

//turns on/off custom pathing. 0 = off, 1 = on.
void toggleCustomPathing(int onOff)
{
	customPathing = onOff; 
}

//changes currentpath pointer to point to specifed path
void setPath(int ID)
{
	if (ID == 1) { currentPath = &waterway; }
	if (ID == 2) { currentPath = &bridge; }
	if (ID == 3) { currentPath = &lakeview; }
}

//returns pathing x cord of specifed index
float next_x(Path *path, int index)
{
	return path->pathPoints[index][0];
}

//returns pathing y cord of specifed index
float next_y(Path *path, int index)
{
	return path->pathPoints[index][1];
}

//returns how many points a path has
int get_totalPathPoints(Path *path)
{
	return path->npathPoints;
}

//clears point array for custom pathing
void resetPath()
{
	memset(pt, 0, sizeof(pt));
	npoint = -1;
}

//used for creating points on right mosue click for the point array
void getCords(int x, int y, int yres)
{
	if (npoint == -1) {
		pt[0].x = x;
		pt[0].y = yres - y;
		npoint = 1;
	} else {
		pt[npoint].x = x;
		pt[npoint].y = yres - y;
		++npoint;
	}
}

//pathing tool. used to display points in point array for easy path coding.
void showCords()
{
	if (npoint < 0) { return; }

	for (int i = 0; i < npoint; i++) {
		if (customPathing) {
			cout << "x cord " << i << " = " << pt[i].x << endl;
			cout << "y cord " << i << " = " << pt[i].y << endl;
			if (i != 0) {
				Vec tmp, tmpTwo;
				tmp[0] = pt[i-1].x;
				tmp[1] = pt[i-1].y;
				tmpTwo[0] = pt[i].x;
				tmpTwo[1] = pt[i].y;
				cout << "distance from " << i-1 << " to " << i << endl;
				cout << " is = " << getDistance(tmp, tmpTwo) << endl << endl;
			}
		}
	}
}

//returns total number of points in point array. used as extern.
int getPointCount()
{
	return npoint;
}
