#include <iostream>
#include "log.h"
#include "fonts.h"

void vananhV(Rect x, int y)
{
    ggprint8b(&x, y, 0x00ff0000, "Jonathan Crawford");
}
