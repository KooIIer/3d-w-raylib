#pragma once
#include <stddef.h>

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    vec3 p1;
    vec3 p2;
    vec3 p3;
    size_t i;
} tri;

typedef struct {
    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 p4;
    size_t i;
} quad;

typedef struct {
    tri t;
    float depth;
} td;
