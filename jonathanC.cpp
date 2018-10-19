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


#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
#include "image.h"

typedef float Flt;
typedef float Vec[3];

Image unitList[1] = {"./images/GIR.jpeg"};


/*
class Global {
public:
	int nslime;
	static Unit slime[MAX_SLIME];
	Global() {
		nslime = 0;
		memset(slime, 0, sizeof(slime));
	}
} s;
*/

//Defines the attributes needed by units within the game.
/*
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

class level {
public:
	Vec spawn[3];
	level() {
		spawn = {0};
	}
}level;

void setSpawn(Flt x, Flt y, Flt z)
{
	level.spawn = {{x}, {y}, {z}};

}
*/

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
	static int wid = 40;
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
void showLevelOne(int x, int y, GLuint textid)
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


Unit createUnit(int i)
{	
	Unit temp;
	int w, h;
	w = unitList[i].width;
	h = unitList[i].height;
	//setup image for the unit.
	glGenTextures(1, &temp.tex);
	glBindTexture(GL_TEXTURE_2D, temp.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, unitList[i].data);

	return temp;
}

void showSlime(Unit s, int x, int y) {

	static int wid = 40;
	glColor3ub(255,255,255);
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, s.tex);
	glTranslated(x/2, y/2, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
