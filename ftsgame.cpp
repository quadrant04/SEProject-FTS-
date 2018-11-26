//modified by:	Ivan Cisneros
//		Ryan Wallace
//		Vananh Vo
//		Jonathan Crawford
//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//mod spring 2018: X11 wrapper class
//This program is a game starting point for a 3350 project.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
//#include <unistd.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "image.h"
#include "stdlib.h" /* malloc in VV file */

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float TIMESLICE = 1.0f;
const float GRAVITY = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const int MAX_SLIME = 100;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

Image credits[4] = {"./images/GIR.jpeg", "./images/ob.jpg", "./images/ic.jpg", "./images/vv.jpg"};
//Image maps[1] = {"./images/firstMap.jpg"};
/* Image units[1] = {"./images/greenslimesprites.gif"}; */
class Global {
public:
	int xres, yres, showCredits, levelOne, spawnSlimeTest, pathingMode;
	char keys[65536];
	GLuint girTexture;
	GLuint obTexture;
	GLuint ivanPicTexture;
	GLuint vvTexture;
	GLuint mapOne;
	GLuint greenSlime;
	Global() {
		xres = 1250;
		yres = 900;
		memset(keys, 0, 65536);
        showCredits = 0;
        levelOne = 0;
        //jwc
        spawnSlimeTest = 0;
        pathingMode = 0;
	}
} gl;

class Game {
public:
	//jwc
	struct timespec slimeTimer;
	int nslimes;
	struct timespec bulletTimer;
public:
	Game() {
		//jwc
		clock_gettime(CLOCK_REALTIME, &slimeTimer);
	}
	~Game() {
		extern void resetSlime();
	}
} g;

//X Windows variables. Class has moved to the header file.
//this should be the only instialized instance of this object
//all others should be "extern" to reference this object.
X11_wrapper x11(gl.xres, gl.yres);

//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//----VV-----------------------------------
extern void animatedSprites(void);
extern void init_background(int w, int h, GLuint texid);
extern void show_bg(int x, int y, GLuint textid);
//----Jonathan-----------------------------
//slime functions
extern void createSlime(int, int, int, int);
extern void showSlime();
extern void moveSlime(int, int, int);
extern void resetSlime();
//pathing functions
extern void getCords(int x, int y, int yres);
extern void showCords();
extern void resetPath();

//----Ivan---------------------------------
//----Ryan---------------------------------

//----All----------------------------------
void show_credits(Rect x, int y); 	

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void init_opengl()
{
	//OpenGL initialization
	glGenTextures(1, &gl.girTexture);
	glGenTextures(1, &gl.obTexture);
	glGenTextures(1, &gl.ivanPicTexture);
	glGenTextures(1, &gl.vvTexture);
	//start of credits----------------------------------------------------------
	//Jonathan's Picture
	int w = credits[0].width;
	int h = credits[0].height;
	glBindTexture(GL_TEXTURE_2D, gl.girTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, credits[0].data);
	//Ryan's Picture
	w = credits[1].width;
	h = credits[1].height;
	glBindTexture(GL_TEXTURE_2D, gl.obTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, credits[1].data);
	//Ivan's Picture
	w = credits[2].width;
	h = credits[2].height;
	glBindTexture(GL_TEXTURE_2D, gl.ivanPicTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, credits[2].data);
	// Vananh's Picture
   	w = credits[3].width;
	h = credits[3].height;
	glBindTexture(GL_TEXTURE_2D, gl.vvTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, credits[3].data);
	//end of credits------------------------------------------------------------
	
	// Initializing background of each status
	//Level 1
	init_background(w, h, gl.mapOne);
	//Start of sprites animation - VV --------------------------------------
	
	void animatedSprites(void);
	
	//End of sprites animation --------------------------------------------
	//
	glViewport(0, 0, gl.xres, gl.yres);
	
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	int mx = e->xbutton.x;
	int my = e->xbutton.y;
	if (e->type != ButtonPress &&
			e->type != ButtonRelease &&
			e->type != MotionNotify)
		return;

	if (e->type == ButtonRelease)
		return;

	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			if (gl.pathingMode)
				getCords(mx , my, gl.yres);
		}
	}

	if (e->type == MotionNotify) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = mx; 
			savey = my;
		}
	}
}

int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	gl.keys[key]=1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_c:
			gl.showCredits ^= 1;
			break;
		case XK_s:
			gl.levelOne ^= 1;
			gl.showCredits = 0;
			break;

		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;

		//jwc
		case XK_p:
			gl.pathingMode ^= 1;
			break;
		case XK_u:
			gl.spawnSlimeTest ^= 1;
			break;
		case XK_r:
			resetSlime();
			resetPath();
			break;
	}
	return 0;
}


void physics()
{

}

void show_credits(Rect x, int y)
{
	glClear(GL_COLOR_BUFFER_BIT);
	extern void jonathanC(Rect x, int y);
	extern void ivanC(Rect x, int y);
	extern void ryanW(Rect x, int y);
	extern void vananhV(Rect x, int y);
	extern void showJonathanPicture(int x, int y, GLuint textid);
	extern void showVananhPicture(int x, int y, GLuint textid);
	extern void showRyanPicture(int x, int y, GLuint textid);
	extern void showIvanPicture(int x, int y, GLuint textid);
	int imagex = gl.xres/3;
    //first
    jonathanC(x, 16);
    showJonathanPicture(imagex, x.bot-30, gl.girTexture);
    //second
    x.bot = gl.yres - 200;
    ryanW(x, 16);
    showRyanPicture(imagex, x.bot-30, gl.obTexture);
    //third
    x.bot = gl.yres - 400;
    ivanC(x, 16);
    showIvanPicture(imagex, x.bot-30, gl.ivanPicTexture);
    //fourth
    x.bot = gl.yres - 600;
    vananhV(x, 16);
    showVananhPicture(imagex, x.bot-30, gl.vvTexture);
}

void showMap() 
{
	int x = gl.xres;
	int y = gl.yres;
	glClear(GL_COLOR_BUFFER_BIT);
	show_bg(x, y, gl.mapOne);
}

void render()
{

	Rect r;
	//y value
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	if (gl.showCredits) {
        show_credits(r, 16);
	} else { 
    		showMap();
    		//jwc
    			if (gl.spawnSlimeTest) {
    				struct timespec st;
				clock_gettime(CLOCK_REALTIME, &st);
				double ts = timeDiff(&g.slimeTimer, &st);
					if (ts > 6.0) {
						timeCopy(&g.slimeTimer, &st);
						createSlime(0, gl.xres, gl.yres, gl.pathingMode);
					}
				showSlime();
				moveSlime(gl.pathingMode, gl.xres, gl.yres);
    			}
    
    	if (gl.pathingMode) {
    			showCords();
    	}
	}
}	
