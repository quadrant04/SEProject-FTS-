//Author: Ryan Wallace
//
//Prints out the names of one member
//


#include "fonts.h"
#include <iostream>
#include <GL/glx.h>
#include <math.h>
using namespace std; 

void ryanW(Rect x, int y) 
{
    ggprint8b(&x, y, 0x00ff0000, "Ryan Wallace");
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
