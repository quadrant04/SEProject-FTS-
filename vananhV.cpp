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

#include "math.h"
#include <GL/glx.h>
#include "fonts.h"

#include <stdlib.h> /* malloc */
#include <X11/keysym.h>
#include <X11/Xlib.h>

Image units[1] = {"./images/greenslimesprites.gif"};

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

/* Convert .gif images into multiple frames
 *  in order to display animated 2D sprites. */
unsigned char *buildAlphaData(Image *units)
{
	// Add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
        unsigned char *data = (unsigned char *)units->data;
        newdata = (unsigned char *)malloc(units->width * units->height * 4);
        ptr = newdata;
        unsigned char a,b,c;
        //use the first pixel in the image as the transparent color.
        unsigned char t0 = *(data+0);
        unsigned char t1 = *(data+1);
        unsigned char t2 = *(data+2);
        for (i=0; i<units->width * units->height * 3; i+=3) {
                a = *(data+0);
                b = *(data+1);
                c = *(data+2);
                *(ptr+0) = a;
                *(ptr+1) = b;
                *(ptr+2) = c;
                *(ptr+3) = 1;
                if (a==t0 && b==t1 && c==t2)
                        *(ptr+3) = 0;
//-----------------------------------------------
                ptr += 4;
                data += 3;
        }
        return newdata;
}
//Load animated images files
int animatedSprites(int width, int height)
{
	// New set data (turn images into data)
	unsigned char *greenSlimeData = buildAlphaData(&units[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, greenSlimeData);
        free(greenSlimeData);
	
	return 0;
}
/*
// Borrow from JC to test.
void showSlime(Unit s, int x, int y) 
{

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
*/
