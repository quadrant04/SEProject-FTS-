//Author: Ryan Wallace
//
//Prints out the names of one member
//


#include "fonts.h"
#include <iostream>
using namespace std; 

void show_credits(int x, int y) {
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Ryan Wallace");
}


