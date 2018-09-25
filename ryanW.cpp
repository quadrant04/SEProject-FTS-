//Author: Ryan Wallace
//
//Prints out the names of one member
//


#include "fonts.h"
#include <iostream>
using namespace std; 

void ryanW(Rect x, int y) {
    x.bot = y;
    x.left = 10;
    x.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Ryan Wallace");
}


