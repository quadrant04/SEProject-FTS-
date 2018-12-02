//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to 
//              display backgrounds and sprites into the game
// --------------------------------------------------------------------
// Recent Modified Date: 10/20/2018
// Description: Attempted to show animated sprites of slime
//
// --------------------------------------------------------------------
// Current Known Bug: animated sprites is not showing yet.
// Date found: 10/20/2018
// --------------------------------------------------------------------
#include "image.h" /* image header file */
#include "jonathanC.h"
#include "vananhV.h"

#include "math.h"
#include <GL/glx.h>
#include "fonts.h"

#include <stdlib.h> /* malloc */
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <iostream>

using namespace std;

Image units[1] = {"./images/greenslimesprites.gif"};
Image towers[1] = {"./images/cannonanimated1kms.gif"};
Image maps[2] = {"./images/FTSTitleScreen.jpg", "./images/firstMap.jpg"};

Frame frame;

static int show = 0;

void vananhV(Rect x, int y)
{
        ggprint8b (&x, y, 0x00a020f0, "Vananh Vo ");
}

void showVananhPicture (int x, int y, GLuint texid)
{
        //show vv picture
        static float angle = 0.0f;
        float fx = (float)x;
        float fy = (float)y;
        double g = 0.0;
        g += sin(angle) * 10.0f;
        fx += g; 
        fy += g + 2.0;
        angle += 0.5f;
        glColor3ub(250, 250, 250);
        int wid=50;
        glPushMatrix();
        glTranslatef(fx,fy,0);
        glBindTexture(GL_TEXTURE_2D, texid);
        glBegin(GL_QUADS);
                   glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                   glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                   glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                   glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
        glEnd();
        glPopMatrix();
}

// Making background of the sprite sheets transparent

unsigned char *buildAlphaData(Image *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a,b,c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data+0);
    unsigned char t1 = *(data+1);
    unsigned char t2 = *(data+2);
    cout << t1;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = 1;
        if (a==t0 && b==t1 && c==t2) {
            *(ptr+3) = 0;
        }
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

// Initialize background
//======================================================//
void init_background(int i, GLuint texid)
{
    int w = maps[i].width;
    int h = maps[i].height;
    glBindTexture(GL_TEXTURE_2D, texid);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                GL_RGB, GL_UNSIGNED_BYTE, maps[i].data);
}
//======================================================//
// Show Background
void show_background(int x, int y, GLuint textid)
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
// ====================================================//

// ========== Initializing Unit =======================//
void init_unit(Unit* p)
{
    int w = units[0].width;
    int h = units[0].height;
    //setup image for the unit.
    glGenTextures(1, &p->tex);
    glBindTexture(GL_TEXTURE_2D, p->tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, units[0].data);
}

// ==================================================//

// ================ Initializing Animated Unit ==========//

void init_animatedUnit(Unit* p)
{
    int w = units[0].width;
    int h = units[0].height;
    //setup image for the unit.
    unsigned char *slimeData = buildAlphaData(&units[0]);

    glGenTextures(1, &p->tex);
    glBindTexture(GL_TEXTURE_2D, p->tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, slimeData);
    free(slimeData);
}
// ========================================================
// =============== Show Unit ==============
void show_unit(float x, float y, GLuint texid)
{
    static int wid = 40;
    glColor3ub(255,255,255);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texid);
    glTranslated(x, y, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
}

// =========================================================

// ============ Show Animated Units ========================

void physics_animation()
{
    if (show == 0) { // For J.C.'s units
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.slimeTimer, &timers.timeCurrent);
        if (timeSpan > frame.delay) {
        //advance
            ++frame.fSlime;
            if (frame.fSlime >= 4)
                frame.fSlime -= 4;
        timers.recordTime(&timers.slimeTimer);
        }
    } else { // For Ryan's tower
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.towerTimer, &timers.timeCurrent);
        if (timeSpan > frame.delay) {
        //advance
            ++frame.fTower;
            if (frame.fTower >= 4)
                frame.fTower -= 4;
        timers.recordTime(&timers.towerTimer);
        }
    }
}

void show_animatedUnit(float x, float y, GLuint texid)
 {
    //static int wid = 40;
    static int wid = 30; // Make the slimes smaller.
    glColor3ub(255,255,255);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texid);

    // changing frames
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslated(x, y, 0); // For JC Pathing
    glBegin(GL_QUADS);
    float ix = frame.fSlime % 2;
    //float fy;
    float fx = ix / 4.0;
    glTexCoord2f(fx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(fx, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(fx+0.25, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(fx+0.25, 1.0f); glVertex2i( wid,-wid);
    
    // end of changing frames
    glEnd();
    glPopMatrix();
      
 }
 // ======================================================
/*
// ============ Display Tower ====================
// Initialization & Building Alpha Image for Tower
void init_animatedTower(Tower* p)
{
    int w = units[0].width;
    int h = units[0].height;
    //setup image for the unit.
    unsigned char *towerData = buildAlphaData(&towers[0]);

    glGenTextures(1, &p->tex);
    glBindTexture(GL_TEXTURE_2D, p->tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, towerData);
    free(towerData);
}
// Display Tower Animation

void show_animatedTower(float x, float y, GLuint texid)
 {
    show = 1;
    //static int wid = 40;
    static int wid = 30; // Make the slimes smaller.
    glColor3ub(255,255,255);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texid);

    // changing frames
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslated(x, y, 0); // For JC Pathing
    glBegin(GL_QUADS);
    float ix = frame.fTower % 2;
    //float fy;
    float fx = ix / 4.0;
    glTexCoord2f(fx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(fx, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(fx+0.25, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(fx+0.25, 1.0f); glVertex2i( wid,-wid);
    
    // end of changing frames
    glEnd();
    glPopMatrix();
      
 }
*/