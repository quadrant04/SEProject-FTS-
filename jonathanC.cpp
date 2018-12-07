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
//#include "jonathanC.h"
using namespace std;

typedef float Flt;
typedef float Vec[3];

void initPaths(int ID);
float getDistance(Vec a, Vec b);
void vecCopy(Vec a, Vec b);
void vecSub(Vec a, Vec b, Vec c);
void vecNormalize(Vec a);

struct Point {
	float x, y, dist;
};

//------copy paste the section below section to jonathanC.h if updated---------

enum {
	TYPE_NORMAL = 0,
	TPYE_FIRE,
	TYPE_TANK
};

class Path {
public:
	int npathPoints;
	Vec pathPoints[20];
	int pathID;
	Path(int p) {
		pathID = p;
		initPaths(pathID);
	}
	
};

static Path waterway(1), bridge(2), lakeview(3);
static Path *currentPath = &waterway;
const int MAX_POINTS = 100;
static Point pt[MAX_POINTS];
static int npoint = -1;
static int customPathing = 0;

int getPointCount();
void toggleCustomPathing(int onOff);
void setPath(int pathID);
float next_x(Path *path, int index);
float next_y(Path *path, int index);
int get_totalPathPoints(Path *path);
void resetPath();
void getCords(int x, int y, int yres);
void showCords();

class Unit {
public:
	int checkpoint, baseHealth, health, baseMoveSpeed, moveSpeed, pathPoints;
	int type;
	Vec pos;
	Vec vel;
	Image *image;
	Path *path;
	GLuint tex;
	int frame, size;
	struct timespec time;
public:
	Unit() {
		type = TYPE_NORMAL;
		path = NULL;
		pathPoints = 1;
		image = NULL;
		checkpoint = 1;
		moveSpeed = 1;
		health = 100;
		frame = 0;
		size = 1;
	}

	void setType(int typeID)
	{
		if (typeID == 0) {
			type = TYPE_NORMAL;
			baseHealth = 100;
			baseMoveSpeed = 2;
			size = 35;
		}

		if (typeID == 1) {
			type = TPYE_FIRE;
			baseHealth = 50;
			baseMoveSpeed = 3;
			size = 30;
		}

		if (typeID == 2) {
			type = TYPE_TANK;
			baseHealth = 200;
			baseMoveSpeed = 1;
			size = 50;
		}
	}

	void setHealth(int bonus)
	{
		health = baseHealth + (baseHealth * bonus);
	}

	void setSpeed(int bonus)
	{
		moveSpeed = baseMoveSpeed + (baseMoveSpeed * bonus);
	}

	void takeDamage(int dmg)
	{
		health -= dmg;
	}

	void setupSlime(int typeID, int healthBonus, int moveBonus)
	{
		setType(typeID);
		setHealth(healthBonus);
		setSpeed(moveBonus);
	}
};
//---------------------------end copy/paste--------------------------

extern void show_animatedUnit(Unit *p);
extern void init_animatedUnit(int i, Unit* p);
const int MAX_SLIME = 40;
static int nslime = -1;
static Unit slime[MAX_SLIME];

void createSlime();
void moveSlime(int xres, int yres);
void deleteSlime(int);
void resetSlime();
void showSlime();
int getSlimeCount();
void increseSpeed();

//----------------vector functions-------------------------------------
void vecNormalize(Vec a) 
{
	float dist = getDistance(a, a);
	a[0] /= dist;
	a[1] /= dist;
}

void vecSub(Vec a, Vec b, Vec c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

void vecCopy(Vec a, Vec b) 
{
	b[0] = a[0];
	b[1] = a[1];
	b[2] = a[2];
}

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
	ggprint8b(&x, y, 0x00ff0000, "-----------------"); 
	ggprint8b(&x, y, 0x00ff0000, "Slime functionality");
	ggprint8b(&x, y, 0x00ff0000, "Pathing");
	ggprint8b(&x, y, 0x00ff0000, "Game flow and logic");
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

int spawnSlime(Unit *p)
{
	return 0;
}

//adds a slime to the array of slimes. 
void createSlime()
{	
	Unit *p; 
	p = &slime[nslime];
	float healthBonus = 0;
	float moveBonus = 0;
	int done = 0;
	int slimeType = rand() % 2;

	if (customPathing) {
		if (npoint > 0) {
			init_animatedUnit(slimeType, p);
			p->path = NULL;
			p->pos[0] = pt[0].x;
			p->pos[1] = pt[0].y;
			p->pos[2] = 0;
			p->pathPoints = 0;
			p->checkpoint = 1;
			nslime++;
			done = 1;
		}
	} else {
		init_animatedUnit(slimeType, p);
		p->path = currentPath;
		p->pos[0] = next_x(currentPath, 0);
		p->pos[1] = next_y(currentPath, 0);
		p->pos[2] = 0;
		p->pathPoints = get_totalPathPoints(currentPath);
		p->checkpoint = 1;
		nslime++;
		done = 1;
	}

	if (done) {
		p->setupSlime(slimeType, healthBonus, moveBonus);
	}
}

//Steps through an array to display all currently generated slimes.
void showSlime() {
	if (nslime < 1)
		return;

	Unit *p;
	for (int i = 0; i < nslime; i++) {
		p = &slime[i];
		show_animatedUnit(p);
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

int getPointCount()
{
	return npoint;
}
