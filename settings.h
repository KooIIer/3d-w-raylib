#pragma once

// #include <stddef.h>
#include <raylib.h>

const int dw = 1080;
const int dh = 1080;

const int fps = 30;
const float dt = 1./fps;

const float ddist = 3.;
//./obj/scrubPine.obj 1500
//./obj/geodesic_classI_2.obj 3
//./obj/geodesic_classIII_2_1.obj 3
//./obj/teapot.obj 150
//./obj/bunny.obj 3

/*const vec3 vertices[] = {
    {0.3,   0.3,    0.3},
    {-0.3,  0.3,    0.3},
    {-0.3,  -0.3,   0.3},
    {0.3,   -0.3,   0.3},
    
    {0.3,   0.3,    -0.3},
    {-0.3,  0.3,    -0.3},
    {-0.3,  -0.3,   -0.3},
    {0.3,   -0.3,   -0.3},
};

const size_t indices[] = {
    0,1,2,3,
    4,5,6,7,
    0,4,5,1,
    3,7,6,2,
    0,3,7,4,
    1,2,6,5,
};

const vec3 normals[] = {
    {0, 0,  1},
    {0, 0,  -1},
    {0, 1,  0},
    {0, -1, 0},
    {1, 0,  0},
    {-1,0,  0},
};*/

const Color colors[] = {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    MAGENTA,
    // PURPLE,
};
