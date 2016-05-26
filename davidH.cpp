// David A. Hernandez II CS335
#include "Structures.h"
//#include <FMOD/fmod.h>
//#include <FMOD/wincompat.h>
//#include <fmod.h>
extern "C" {
#include "fonts.h"
}
#include <string.h>

using namespace std;
//Converts an image with any file extension imported into 
//images/runner/ directory into a ppm image
string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = path + filename + ".ppm";

    //convert ./images/runner/runner_sheet2.ppm ./images/runner/runner_sheet2.png
    string newstuff =  "convert " + oldfile + " " + newfile;
    filename = system(newstuff.data());
    return filename;
}

