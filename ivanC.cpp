// Author: Ivan Cisneros
// This file will show my name in the credits for our project

#include <iostream>
#include <fonts.h>
using namespace std;

void show_credits(int x, int y)
{
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Ivan Cisneros");
}
