#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
////#include <GL/gl.h>
////#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"


//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
//double backgroundx = 0, spritesheetx = 0;

typedef struct t_bigfoot {
        Vec pos;
        Vec vel;
} Bigfoot;

