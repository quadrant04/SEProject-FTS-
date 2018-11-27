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

void vecCopy(Vec a, Vec b);
void vecSub(Vec a, Vec b, Vec c);
void vecNormalize(Vec a);
float getDistance(Vec a, Vec b);
int getPointCount();
int getSlimeCount();
void deleteSlime(int);

struct Point {
	float x, y, dist;
};

Image unitList[1] = {"./images/GIR.jpeg"};
extern X11_wrapper x11;
extern void init_unit(Unit*);
extern void show_unit(float x, float y, GLuint texid);

/*------------All of my classes can be found in my header file jonathanC.h----------

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
-----------------------------------------------------------------------------------*/
//-----slime variables-----
const int MAX_SLIME = 20;
static int nslime = 0 ;
static Unit slime[MAX_SLIME];

//-----pathing variables---
const int MAX_POINTS = 100;
static Point pt[MAX_POINTS];
static int npoint = 0;
static int seen = 0;
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
//adds a slime to the array of slimes. 
//An int is given dictating which image to load.
void createSlime(int i, int x, int y, int pathing)
{	
	Unit *p; 
	p = &slime[nslime];
	init_unit(p);

	if (pathing && npoint > 0) {
		cout << "Pathing: ON " << endl;
		p->pos[0] = pt[0].x;
		p->pos[1] = pt[0].y;
		cout << "start x = " << p->pos[0] << endl;
		cout << "start y = " << p->pos[1] << endl;
        p->checkpoint++;
	} else {
		p->pos[0] = x/2;
		p->pos[1] = y/2;
		cout << "Pathing: OFF " << endl;
		cout << "start x = " << p->pos[0] << endl;
		cout << "start y = " << p->pos[1] << endl;
		p->vel[0] = 4;
	    p->vel[1] = 0;
	}
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
        show_unit(x,y,p->tex);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

//adds velocity to slimes for movement. 
//current position = intial position+velocity.
//might add a timer element later(pos=pos+(time*vel))
void moveSlime(int pathing, int xres, int yres)
{
	if (nslime == 0 || npoint < 2)
		return;

	for (int i = 0; i < nslime; i++) {
	
		Unit *s;
        s = &slime[i];
		//Check 1:pathing on 
		//check 2: number of checkpoints has to be less then total number of 
		//points in the path.
		//check 3: only move to the next point if the slime is close enough to
		//the point it is currently moving to. 
		if (pathing && 
			(s->checkpoint != npoint) && 
			(s->pos[2] < 20)) {
			
			Vec v, p = {0};
				             
			p[0] = pt[s->checkpoint].x;
			p[1] = pt[s->checkpoint].y;
			vecSub(p, s->pos, v);
			cout << "Before normalized" << endl;
			cout << "v.x = " << v[0] << endl;
			cout << "v.y = " << v[1] << endl;
			vecNormalize(v);
			cout << "After normalized" << endl;
			cout << "v.x = " << v[0] << endl;
			cout << "v.y = " << v[1] << endl;
			vecCopy(v, s->vel);
			s->pos[2] = getDistance(s->pos, p);
			s->checkpoint += 1;
		}
		s->pos[0] += s->vel[0];
		s->pos[1] += s->vel[1];
		s->pos[2] -= 1;		
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

void deleteSlime(int s)
{
    slime[s] = slime[nslime-1];
    nslime--;
    
}

//deletes all the slimes in current array.
//will be called when reseting a level, changing levels/menus,
//or on exiting the program.
void resetSlime()
{
	memset(slime, 0, sizeof(slime));
	nslime = 0;
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

//not used yet.
void getPath(int level, int path)
{}

//clears the pt array, resets the npoint counter.
void resetPath()
{
	memset(pt, 0, sizeof(pt));
	npoint = 0;
}

//saves the cords when the right mouse button is clicked.
//used to find cords for pathing information.
void getCords(int x, int y, int yres) 
{
    seen = 0;
	pt[npoint].x = x;
	pt[npoint].y = yres - y;
	++npoint;
	cout << "n = " << npoint << endl;
	cout << "mx = " << x << endl;
	cout << "my = " << y << endl;	
}

//show cords currently saved in the pt[] array.
//function uses a bool var (seen) so it will not
//display cords everytime render is called, but only when
//a new cord is added.
void showCords()
{
	for (int i = 0; i < npoint; i++) {
		if (!seen) {
			cout << "x cord " << i << " = " << pt[i].x << endl;
			cout << "y cord " << i << " = " << pt[i].y << endl;
		}
	}
	//controls when cords have been updated to display them.
	seen = 1;
}

int getPointCount()
{
    return npoint;
}