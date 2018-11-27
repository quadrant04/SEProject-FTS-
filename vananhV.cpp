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
#include "jonathanC.h" /* For unit stuff */

#include "math.h"
#include <GL/glx.h>
#include "fonts.h"

#include <stdlib.h> /* malloc */
#include <X11/keysym.h>
#include <X11/Xlib.h>

//Image units[1] = {"./images/gslime.gif"};
Image units[1] = {"./images/greenslime.gif"};
Image maps[1] = {"./images/firstMap.jpg"};

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

// Initialize background
//======================================================//
void init_background(GLuint texid)
{
    int w = maps[0].width;
    int h = maps[0].height;
    glBindTexture(GL_TEXTURE_2D, texid);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                GL_RGB, GL_UNSIGNED_BYTE, maps[0].data);
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
// =============== Show Unit ==============
void show_unit(float x, float y, GLuint texid)
{
        static int wid = 40;
        glColor3ub(255,255,255);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texid);
        glTranslated(x/2, y/2, 0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

        glEnd();
        glPopMatrix();
        //glBindTexture(GL_TEXTURE_2D, 0);
}
