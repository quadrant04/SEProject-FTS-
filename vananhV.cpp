//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to display backgrounds and sprites into the game

#include <iostream>
#include <GL/glx.h>
#include <font.h>
using namespace std;

void showVananhName(int x, int y)
{	
	r.bot =gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b (&r, 16, 0x00ff0000, "Art Design: Vananh Vo \n");
}
