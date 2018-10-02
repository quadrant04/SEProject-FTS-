//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to display backgrounds and sprites into the game

#include "math.h"
#include <GL/glx.h>
#include "fonts.h"
using namespace std;

void vananhV(Rect x, int y)
{
        ggprint8b (&x, y, 0x00ff0000, "Vananh Vo ");
}

void showVananhPicture (int x, int y, GLuint texid)
{
        //show vv picture
        static float angle = 2.0f;
        float fx = (float)x;
        float fy = (float)y;
        fx += sin(angle) * 100.0f;
        fy += sin(angle) * 100.0f;
        angle += 0.5f;
        glColor3ub(250, 250, 250);
        int wid=40;
        glPushMatrix();
        glTranslatef(x,y,0);
        glBindTexture(GL_TEXTURE_2D, texid);
        glBegin(GL_QUADS);
                   glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                   glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                   glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                   glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
        glEnd();
        glPopMatrix();
}            
