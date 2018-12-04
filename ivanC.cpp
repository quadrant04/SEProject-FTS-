// Author: Ivan Cisneros
// This file will show my name in the credits for our project
// -Show my name in the credits for our project (Completed date)
// -Display button functionality
//  Edit:  Display options (11/27/2018)
//         12/01/2018 - Displaying menu bar
//

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
#include <stdlib.h> /* malloc */
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include "image.h" 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

//Images that will be displayed for toggling sound and showing win/loss
/*
Image icons[3] = {"./images/time.png", "./images/prize.png", 
	          "./images/rip.png"};
*/
//------------------------------------------------------------------------------

//------------ Extern functions for slime and point # --------------------------
extern int getPointCount();
extern int getSlimeCount();
extern X11_wrapper x11;
//------------------------------------------------------------------------------

//---------------------Ivan's credit functions----------------------------------
void ivanC(Rect x, int y)
{
  ggprint8b(&x, 16, 0x00ff0000, "Ivan Cisneros");
}

void showIvanPicture(int x, int y, GLuint textid)
{
  //show bat picture
  static float angle = 0.0f;
  float fx = (float)x;
  float fy = (float)y;
  double a = 0.0;
  a+= sin(angle) * 10.0f;
  fx += a;
  angle += 0.2f;
  glColor3ub(255, 255, 255);
  int wid=40;
  glPushMatrix();
  glTranslatef(fx,fy,0);
  glBindTexture(GL_TEXTURE_2D, textid);
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
  glEnd();
  glPopMatrix();
}
//------------------------------------------------------------------------------

//------------------------Button options----------------------------------------
void boxMenu(int x)
{
  glPushMatrix();
  glTranslatef(x, 375.0f, 0.0f);
  glColor3f(0.1f, 0.6f, 0.1f);
  glBegin(GL_QUADS);
    glVertex2f(240.0, 290.0);
    glVertex2f(-4.0, 290.0);
    glVertex2f(-4.0, 510.0);
    glVertex2f(240.0, 510.0);
  glEnd();
  glPopMatrix();
}
void boxPath(int x)
{
  glPushMatrix();
  glTranslatef(x, 145.0f, 0.0f);
    glColor3f(0.1f, 0.6f, 0.1f);
  glBegin(GL_QUADS);
    glVertex2f(237.0, 360.0);
    glVertex2f(-4.0, 360.0);
    glVertex2f(-4.0, 500.0);
    glVertex2f(237.0, 500.0);
  glEnd();
  glPopMatrix();
}

void showButtonOptions(Rect x, int xres, int slimeToggle, int pathing) {
  int color = 0xffffff;
  char slimeONOFF[30];
  char pathingONOFF[30]; 	
  char slimeCount[30];
  char pointCount[30];
  int pCount = getPointCount(); 
  
  x.left = xres - 300;
  boxMenu(x.left);

  if (slimeToggle) {
    sprintf(slimeONOFF, "U - Slime: ON ");

  } else {
    sprintf(slimeONOFF, "U - Slime: OFF ");
		}
  if (pathing) {
    sprintf(pathingONOFF, "P - Pathing: ON ");

  } else {
    sprintf(pathingONOFF, "P - Pathing: OFF ");
		}
  
  sprintf(slimeCount, "# of Slimes: %d", getSlimeCount());
  sprintf(pointCount, "# of Points: %d", pCount);
  	
  ggprint16(&x, 26, color, "");
  ggprint16(&x, 26, color, slimeONOFF);
  ggprint16(&x, 26, color, slimeCount);
  ggprint16(&x, 26, color, "^ - Increase Slime Speed");
  ggprint16(&x, 26, color, "v - Decrease Slime Speed");
  ggprint16(&x, 26, color, "R - Reset pathing of slimes");
  ggprint16(&x, 26, color, "N - Title Screen");
  ggprint16(&x, 26, color, "C - Credits");
  ggprint16(&x, 26, color, pathingONOFF);
  
  //ggprint8b(&x, 16, 0x00ff0000, "1 - Toggle Sound");
  
  if (pathing) {
    boxPath(x.left);
    ggprint16(&x, 26, color, "");
    ggprint16(&x, 26, color, pointCount);
    ggprint16(&x, 26, color, "Right Click To Add");
    ggprint16(&x, 26, color, "S - Show Chords");
    ggprint16(&x, 26, color, "T - Towers");
    
    if (pCount > 0) {
      ggprint16(&x, 26, color, "Initial spawn point: ON");
    } else {
      ggprint16(&x, 26, color, "Initial spawn point: OFF");
    }
  }
}

//------------------------------------------------------------------------------

//----------------------Display # of slimes and points--------------------------
void showMenu(Rect x, int y)
{
  int BLACK = 0x000000;
  int RED = 0xff0000;
	x.left = 1;
	x.center = 0;

  ggprint13(&x, 16, BLACK, "Press M For Options ");
  ggprint13(&x, 16, BLACK, "");
  ggprint13(&x, 16, BLACK, "Press Esc to Exit Game");
  ggprint13(&x, 16, RED, "And Doom The Slimes! ");
}

//------------------------------------------------------------------------------

//---------------------------Sound functions------------------------------------
void* introMusic(void* arg)
{
  if (arg) {

  }

  #ifdef USE_OPENAL_SOUND
  alutInit(0, NULL);
  if (alGetError() != AL_NO_ERROR) {
    printf("ERROR: alutInit()\n");
    return 0;
  }
  //Clear error state.
  alGetError();
  //Setup the listener.
  //Forward and up vectors are used.
  float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alListenerfv(AL_ORIENTATION, vec);
  alListenerf(AL_GAIN, 1.0f);
  
  //Buffer holds the sound information.
  ALuint alBuffer;
  alBuffer = alutCreateBufferFromFile("sounds/ftsintro.wav");
  //Source refers to the sound.
  ALuint alSource;
  //Generate a source, and store it in a buffer.
  alGenSources(1, &alSource);
  alSourcei(alSource, AL_BUFFER, alBuffer);
  //Set volume and pitch to normal, no looping of sound.
  alSourcef(alSource, AL_GAIN, 1.0f);
  alSourcef(alSource, AL_PITCH, 1.0f);
  alSourcei(alSource, AL_LOOPING, AL_FALSE);
  
  if (alGetError() != AL_NO_ERROR) {
    printf("ERROR: setting source\n");
    alutExit();
    pthread_exit(NULL);
  }

  #endif
  
  pthread_exit(NULL);
}

void* levelMusic(void* arg)
{
  #ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
	  if (alGetError() != AL_NO_ERROR) {
		  printf("ERROR: alutInit()\n");
		  return 0;
	  }
	  //Clear error state.
	  alGetError();
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sounds/level1.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
  }

  #endif
  
  pthread_exit(NULL);
}

/*
int button (int x, int y)
{
    // New Game coordinates:
    //  botLeft: x-498. y-436
    //  topLeft: x-498, y-468
    //  topRight: x-765, y-469
    //  botRight: x-765, y-436

    // Credit coordinates:
    //  botLeft: x-524. y-317
    //  topLeft: x-524, y-351
    //  topRight: x-735, y-351
    //  botRight: x-735, y-318

   // Quit coordinates:
    //  botLeft: x-573. y-205
    //  topLeft: x-573, y-245
    //  topRight: x-688, y-244
    //  botRight: x-688, y-207

  //new game

}
// use save x, save y in button()
// int button(int x, int y)
// if(x > leftmostx && x < rightmost x && y < topmosty && y > bottommost y)
//   return 0;
// g.titlenewgameButon()

//check mouse
// int button = 0
//  if button == 1
//  if button == 2
//  if button == 3
*/
