#include <iostream> // for cout
#include <unistd.h> // for close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
extern "C" {
#include "fonts.h"
}
using namespace std;


#define HOST "sleipnir.cs.csubak.edu"
#define PAGE "~tmorrell/335/templerun/Users.php"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"

int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void usage();
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
//double backgroundx = 0, spritesheetx = 0;

typedef struct t_bigfoot {
        Vec pos;
        Vec vel;
} Bigfoot;

// prototypes
void projectImage(float x, float y, float z, GLuint Texture);
int checkcollison(int sprite_x, float x, int sprite_y, float y);
