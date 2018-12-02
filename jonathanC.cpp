//Author: Jonathan Crawford
//
//two functions. One displays the authors name when called using openGl.
//the second, displays a picture representing the author when called.
//
//Recent: Added global variables and functions calls to from main to this file
//to display level one. In the future, going to create "game states" so the 
//actual game pauses in the background when credits is shown.
//
//current bug, level one displays red intially.

/*-------------------------------------------------------------------
//Defines the attributes needed by units within the game.
//Unit class is defined within the header file. This is for reference.

class Unit {
public:
	int onoff;
	int frame;
	double delay;
	Vec pos;
	Vec vel;
	Image *image;
	GLuint tex;
	struct timespec time;
	Unit() {
		onoff = 0;
		frame = 0;
		image = NULL;
		delay = 0.1;
	}
};
---------------------------------------------------------------------*/

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
extern void initPaths(int ID);

/*
class Unit {
public:
	int onoff;
	int checkpoint;
	int frame;
	double delay;
	Vec pos;
	Vec vel;
	Image *image;
	GLuint tex;
	struct timespec time;
	Unit() {
		onoff = 0;
		frame = 0;
		image = NULL;
		delay = 0.1;
		checkpoint = 1;
	}
};

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
*/

Path waterway(1);


//----vector functions----
void vecCopy(Vec a, Vec b);
void vecSub(Vec a, Vec b, Vec c);
void vecNormalize(Vec a);
float getDistance(Vec a, Vec b);

//-----slime variables-----
const int MAX_SLIME = 20;
static int nslime = -1;
static Unit slime[MAX_SLIME];
static int speed = 1;
static Path *currentPath = &waterway;

//-----slime functions-----
void createSlime();
void moveSlime(int xres, int yres);
void deleteSlime(int);
void resetSlime();
void showSlime();
int getSlimeCount();
void increseSpeed();

//-----pathing variables---
const int MAX_POINTS = 100;
static Point pt[MAX_POINTS];
static int npoint = -1;
static int seen = 0;
static int customPathing = 0;

//-----pathing functions---
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

//normalizes a given vector. vector components devided by
//total distance of given vector. distance =sqrt(a^2 + b^2)
//x = x/dist, y = y/dist.
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

//finds and returns the distance between two vectors.
//if the same vector is given twice, finds and returns 
//the distance of the single vector.
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
//displays the author's name (Jonathan Crawford) using the React obj to 
//determin the x value, and the y vale is given to the function. 
void jonathanC(Rect x, int y)
{
	ggprint8b(&x, y, 0x00ff0000, "Jonathan Crawford");
}

//Display an image to the screen
//using the texture id, x and y pos given to the function.
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

//Displays the first map of the game using the current resolution (x, y), as 
//well as the map image with the gluint texture id.
/*void showLevelOne(int x, int y, GLuint textid)
{
	//map setup.
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,255);
	glPushMatrix();
	//glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, textid);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);

	glEnd();
	glPopMatrix();
}
*/
//----------------------------slime functions----------------------------

//this modifier will be multipled by velocity when moving the slimes.
void increseSpeed()
{
	speed += 1;
}

//adds a slime to the array of slimes. 
//An int is given dictating which image to load.
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
		}   
	} else {
		s->path = currentPath;
		s->pos[0] = next_x(currentPath, 0);
		s->pos[1] = next_y(currentPath, 0);
		s->pos[2] = 0;
		s->pathPoints = get_totalPathPoints(currentPath);
	}

	s->moveSpeed = speed;
	s->checkpoint = 1;
	nslime++;
	return;
}

//Steps through an array to display all currently generated slimes.
void showSlime() {
    //check to see if slimes present.
    if (nslime < 1)
        return;
    Unit *p;
    //this code may be modified later for animation purposes.
    for (int i = 0; i < nslime; i++) {
        p = &slime[i];
        float x = p->pos[0];
        float y = p->pos[1];
        //show_unit(x,y,p->tex);
        show_animatedUnit(x,y,p->tex);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

//adds velocity to slimes for movement. 
//current position = intial position+velocity.
//might add a timer element later(pos=pos+(time*vel))
void moveSlime(int xres, int yres)
{
	for (int i = 0; i < nslime; i++) {
	
		Unit *s;
        s = &slime[i];
        int sp = s->moveSpeed;
		//Check 1:pathing on 
		//check 2: number of checkpoints has to be less then total number of 
		//points in the path.
		//check 3: only move to the next point if the slime is close enough to
		//the point it is currently moving to. 
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

//deletes a single slime. if the slime is NOT the last one, overrites the slime that needs to
//be deleted with the last slime in the array and decreases total slimes by 1.
//NOTE: the data in the array is not reset, so needs to be overritten next time a slime is created.
//if the slime is the last slime in the array, simply decreases total slimes by 1, so it will not render.
void deleteSlime(int s)
{

    if (s == (nslime-1)) {
        nslime -= 1;
        return;
    }
    slime[s] = slime[nslime-1];
    nslime -= 1;
    
}

//deletes all the slimes in current array.
//will be called when reseting a level, changing levels/menus,
//or on exiting the program.
void resetSlime()
{
	memset(slime, 0, sizeof(slime));
	nslime = -1;
}

int getSlimeCount()
{
    return nslime;
}

//------------------Pathing Mode------------------------------------
//this functionality is designed to be able to create a path by 
//allowing the creation of points and lines, and seeing the x and y info
//in the teriminal window.
//Possibly will be able to add the feature where users could
//defin their own paths and simply restrict them to certain areas.

void initPaths(int ID)
{	
	/*
	waterway[0][0] = 225;  
	waterway[0][1] = 262; 
	waterway[0][2] = 8;
	
	waterway[1][0] = 228;  
	waterway[1][1] = 183; 
	waterway[1][2] = 79.06;
						
	waterway[2][0] = 517;  
	waterway[2][1] = 186; 
	waterway[2][2] = 289.01;
					
	waterway[3][0] = 516;  
	waterway[3][1] = 388; 
	waterway[3][2] = 202.0;
						
	waterway[4][0] = 703;  
	waterway[4][1] = 386; 
	waterway[4][2] = 187.01;
						
	waterway[5][0] = 709;  
	waterway[5][1] = 271; 
	waterway[5][2] = 115.16;
						
	waterway[6][0] = 1114; 
	waterway[6][1] = 261;
	waterway[6][2] = 405.12;
						
	waterway[7][0] = 1115; 
	waterway[7][1] = 15;
	waterway[7][2] = 246.0;
	*/
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
	}	
}

//toggles custom pathing. Only the move slime function uses this variable
void toggleCustomPathing(int onOff)
{
	customPathing = onOff; 
}

//this will dictate what path slimes will take on creation.
void setPath(int ID)
{
	if (ID == 1) { currentPath = &waterway; }
}

//gets the specifed x cord from a path object
//this is the first variable in a vec. (i.e. vert[*][0])
float next_x(Path *path, int index) 
{
		return path->pathPoints[index][0];
}

//gets the specifed y cord from a path object
//this is the second variable in a vec. (i.e. vert[*][1])
float next_y(Path *path, int index) 
{
		return path->pathPoints[index][1];
}

int get_totalPathPoints(Path *path)
{
	return path->npathPoints;
}


//clears the pt array, resets the npoint counter.
void resetPath()
{
	memset(pt, 0, sizeof(pt));
	npoint = -1;
}

//saves the cords when the right mouse button is clicked.
//used to find cords for pathing information.
void getCords(int x, int y, int yres) 
{
    seen = 0;

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

//show cords currently saved in the pt[] array.
//function uses a bool var (seen) so it will not
//display cords everytime render is called, but only when
//a new cord is added.
void showCords()
{
	if (npoint < 0) { return; }

	for (int i = 0; i < npoint; i++) {
		if (!seen && customPathing) {
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
	//controls when cords have been updated to display them.
	seen = 1;
}

int getPointCount()
{
    return npoint;
}
