#include <iostream>
#include <cstring>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

void show_credits(Rect x, int y)
{
    ggprint8b(&x, y, 0x00ff0000, "Jonathan Crawford");
}
