//Author: Ryan Wallace
//
//Prints out the names of one member
//


#include "fonts.h"
#include <iostream>
using namespace std; 

void ryanW(Rect x, int y) 
{
    ggprint8b(&x, 16, 0x00ff0000, "Ryan Wallace");
}


