#ifndef TRIANGLE_H
#define TRIANGLE_h

#include "vec.h"

// just like an EBO in OpenGL :)
typedef struct {
    int a;
    int b;
    int c;
} face_t;

// stores projected points of triangle
typedef struct {
    vec2_t points[3];
} triangle_t;

#endif