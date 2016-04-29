// David A. Hernandez II CS335
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
using namespace std;

//Defined Types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];

typedef struct t_bigfoot {
    Vec pos;
    Vec vel;
} Bigfoot;


string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = filename + ".ppm";
    filename = system("convert path newfilename");
//    system(convert  strcat(filename, ".ppm"));
    return filename;

}
    



