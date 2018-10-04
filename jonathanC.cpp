//Author: Jonathan Crawford
//
//two functions. One displays the authors name when called using openGl.
//the second, displays a picture representing the author when called.


#include "fonts.h"
#include <GL/glx.h>
#include <math.h>

void jonathanC(Rect x, int y)
{
    ggprint8b(&x, y, 0x00ff0000, "Jonathan Crawford");
}

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
