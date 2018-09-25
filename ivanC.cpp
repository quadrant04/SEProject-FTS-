// Author: Ivan Cisneros
// This file will show my name in the credits for our project

#include <iostream>
#include "fonts.h"
using namespace std;

void ivanC(Rect x, int y)
{
    ggprint8b(&x, 16, 0x00ff0000, "Ivan Cisneros");
}
