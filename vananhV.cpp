//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to display backgrounds and sprites into the game

#include "math.h"
#include <GL/glx.h>
#include "fonts.h"
using namespace std;

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
