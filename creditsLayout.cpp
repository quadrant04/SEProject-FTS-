//Author: Ryan Wallace
//Date created: 27 sept 2018
//purpose: displays a layout for the credits screen
//


#include "fonts.h"
#include <iostream>
using namespace std; 

void creditsLayout(Rect x, int y)
{
    ggprint8b(&x, y, 0x0000ff00, "CREDITS");
}
