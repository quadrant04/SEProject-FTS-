// Author: Ivan Cisneros
// This file will show my name in the credits for our project

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
using namespace std;

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

