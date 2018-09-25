// Author: Ivan Cisneros
// This file will show my name in the credits for our project

#include <iostream>
#include "fonts.h"
using namespace std;

void show_credits(Rect x, int y)
{
    x.bot = y;
    x.left = 10;
    x.center = 0;
    ggprint8b(&x, 16, 0x00ff0000, "Ivan Cisneros");
}
