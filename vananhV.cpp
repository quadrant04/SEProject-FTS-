//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to display backgrounds and sprites into the game

#include <iostream>
#include <cstring>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
using namespace std;

void showVananhName(Rect x, int y)
{	
	ggprint8b (&x, y, 0x00ff0000, "Art Design: Vananh Vo \n");
}
