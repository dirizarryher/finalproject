#ifndef _PPM_H_
#define _PPM_H_

#define MAX_PARTICLES 50000



//game.h
//Read and write PPM files. Plain format.
//#include <sys/types.h>
//Structures
typedef struct Vec {
        float x, y, z;
} Vec;

typedef struct Shape {
        float width, height;
        float radius;
        Vec center;
} Shape;

typedef struct Particle {
        Shape s;
        Vec velocity;
} Particle;

typedef struct Game {
        Shape box[5];
        Shape circle[2];
        Particle particle[MAX_PARTICLES];
        int n;
        int lastMousex;
        int lastMousey;
} Game;

#endif

