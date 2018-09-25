//Author: Vananh Vo
//Date: 09/25/2018
//Description: This file will include how to display backgrounds and sprites into the game

#include <iostream>
#include "log.h"
#include "fonts.h"
using namespace std;

void vananhV(Rect x, int y)
{	
	ggprint8b (&x, y, 0x00a020f0, "Vananh Vo ");
}
