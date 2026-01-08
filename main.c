#include <math.h>
#include <rlgl.h>
#include <raylib.h> 
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "settings.h"
#include "model.h"

/*typedef struct {
    size_t size;
    quad* values;
} qda;*/

// quad depths[sizeof(indices)/sizeof(size_t)/4];

// qda depths = {0};

float tc = 0.;
float sina = 0.;
float cosa = 0.;

bool debug = false;
bool culling = false;
bool transparent = false;

float dist = ddist;

int w = dw;
int h = dh;

vec2 project(vec3 p) {
    return (vec2){p.x/p.z, p.y/p.z};
}

vec2 screen(vec2 p) {
    return (vec2){(p.x+1)*w/2, (1-(p.y+1)/2)*h};
}

vec3 rotatey(vec3 p) {
    return (vec3){p.x*cosa-p.z*sina, p.y, p.x*sina+p.z*cosa};
}

vec3 rotatez(vec3 p) {
    return (vec3){p.x*cosa-p.y*sina, p.x*sina+p.y*cosa, p.z};
}

vec3 rotatex(vec3 p) {
    return (vec3){p.x, p.y*cosa - p.z*sina, p.y*sina+p.z*cosa};
}

vec3 camera(vec3 p) {
    return (vec3){p.x, p.y, p.z+dist};
}

vec3 convert(vec3 p) {
    return rotatey(rotatex(p));
}

vec2 drawable(vec3 p) {
    return screen(project(camera(p)));
}

bool facing(size_t i) {
    if (!culling) {
        return false;
    }

    vec3 normal = convert(normals[i]);

    // printf("%d", (int)(normal.z > 0));

    return normal.z > 0;
}

void drawTri(tri t) {
    if (facing(t.i)) { return; }

    const vec2 p1 = drawable(t.p1);
    const vec2 p2 = drawable(t.p2);
    const vec2 p3 = drawable(t.p3);

    // Color color = colors[t.i%(sizeof(colors)/sizeof(Color))];
    // Color color = {t.i, 0, 0, 255};
    // Color color = {rand()%255, rand()%255, rand()%255, 255};
    // printf("%zu\n", t.i);

    srand(t.i+1);
    Color color = {rand()%255, rand()%255, rand()%255, 255};

    DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, (Vector2){p3.x, p3.y}, color);
}

void drawQuad(quad q) {
    if (facing(q.i)) { return; }
    // printf("%zu", q.i);

    const vec2 p1 = drawable(q.p1);
    const vec2 p2 = drawable(q.p2);
    const vec2 p3 = drawable(q.p3);
    const vec2 p4 = drawable(q.p4);

    // DrawText(TextFormat("%zu", q.i), p1.x, p1.y, 20, WHITE);
   
    // Color color = colors[q.i%(sizeof(colors)/sizeof(Color))];
    // Color color = {q.i, 0, 0, 255};

    srand(q.i+1);
    Color color = {rand()%255, rand()%255, rand()%255, 255};
    
    DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, (Vector2){p3.x, p3.y}, color);
    DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p3.x, p3.y}, (Vector2){p4.x, p4.y}, color);

    // spr.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, colors[q.i%(sizeof(colors)/sizeof(uint16_t))]);
    // spr.fillTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, colors[q.i%(sizeof(colors)/sizeof(uint16_t))]);
}

float avgdepthQuad(quad* q) {
    return (q->p1.z+q->p2.z+q->p3.z+q->p4.z)/4;
}

float avgdepthTri(tri* t) {
    return (t->p1.z+t->p2.z+t->p3.z)/3;
}

/*bool quad_le(quad* q1, quad* q2) {
    // return (q1->p1.z+q1->p2.z+q1->p3.z+q1->p4.z)/4 <= (q2->p1.z+q2->p2.z+q2->p3.z+q2->p4.z)/4;
    return avgdepth(q1) >= avgdepth(q2);
}*/

//наверное можно убрать и сделать !quad_le
/*bool quad_g(quad* q1, quad* q2) {
    return (q1->p1.z+q1->p2.z+q1->p3.z+q1->p4.z)/4 > (q2->p1.z+q2->p2.z+q2->p3.z+q2->p4.z)/4;
}*/

/*void debugDepths() {
    for (size_t i = 0; i < depths.size; i++) {
        printf("%zu: (%zu) %f\n", i, depths.values[i].i, avgdepth(&depths.values[i]));
    }
}
*/

//лучше не смотреть на этот ужас
/*qda qdasort(qda* arr) {
    if (arr-> size == 0) {
        return (qda){0};
    }
    if (arr->size == 1){
        // return *arr;
        qda r = {0};
        r.values = (quad*)malloc(sizeof(quad));
        r.values[0] = arr->values[0];
        r.size = 1;
        return r;
    }
    int p = (int)arr->size/2;
    qda left = {0};
    qda right = {0};
    qda newarr = {0};
    left.values = (quad*)malloc(arr->size*sizeof(quad));
    for (int i = 0; i < arr->size; i++) {
        if (quad_le(&arr->values[i], &arr->values[p]) && i != p) {
            left.values[left.size++] = arr->values[i];
        }
    }
    qda left_s = qdasort(&left);
    right.values = (quad*)malloc(arr->size*sizeof(quad));
    for (int i = 0; i < arr->size; i++) {
        // if (quad_g(&arr->values[i], &arr->values[p]) && i != p) {
        if (!quad_le(&arr->values[i], &arr->values[p]) && i != p) {
            right.values[right.size++] = arr->values[i];
        }
    }
    newarr.values = (quad*)malloc(arr->size*sizeof(quad));
    for (int i = 0; i < left_s.size; i++) {
        newarr.values[newarr.size++] = left_s.values[i];
    }
    qda right_s = qdasort(&right);
    newarr.values[newarr.size++] = arr->values[p];
    for (int i = 0; i < right_s.size; i++) {
        newarr.values[newarr.size++] = right_s.values[i];
    }
    free(left.values);
    free(right.values);

    free(left_s.values);
    free(right_s.values);

    return newarr;
}*/

void dsortQuad(quad* d, size_t l) {
    // NOLINTBEGIN(bugprone-sizeof-expression)
    for (size_t i = 0; i < l; i++) {
        for (size_t j = i+1; j < l; j++) {
            // if (quad_le(&d[i], &d[i])) {
            if (avgdepthQuad(&d[i]) < avgdepthQuad(&d[j])) {
                quad q = d[i];
                d[i] = d[j];
                d[j] = q;

            }
        }
    }
    // NOLINTEND(bugprone-sizeof-expression)
}

/*void dsortTri(tri* d, size_t l) {
    // NOLINTBEGIN(bugprone-sizeof-expression)
    for (size_t i = 0; i < l; i++) {
        for (size_t j = i+1; j < l; j++) {
            // if (quad_le(&d[i], &d[i])) {
            if (avgdepthTri(&d[i]) < avgdepthTri(&d[j])) {
                tri t = d[i];
                d[i] = d[j];
                d[j] = t;

            }
        }
    }
    // NOLINTEND(bugprone-sizeof-expression)
}*/

void hd(tri* d, size_t n, size_t i) {
    size_t mx = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && avgdepthTri(&d[left]) > avgdepthTri(&d[mx]))
        mx = left;

    if (right < n && avgdepthTri(&d[right]) > avgdepthTri(&d[mx]))
        mx = right;

    if (mx != i) {
        tri tmp = d[i];
        d[i] = d[mx];
        d[mx] = tmp;

        hd(d, n, mx);
    }
}

void dsortTri(tri* d, size_t l) {
    if (l < 2) return;
    
    for (size_t i = l / 2; i > 0; i--) {
        hd(d, l, i - 1);
    }
    
    for (size_t i = l - 1; i > 0; i--) {
        tri tmp = d[0];
        d[0] = d[i];
        d[i] = tmp;

        hd(d, i, 0);
    }

    for (size_t i = 0; i < l / 2; i++) {
        tri tmp = d[i];
        d[i] = d[l - 1 - i];
        d[l - 1 - i] = tmp;
    }
}

void modelQuad() {
    quad depths[sizeof(indices)/sizeof(size_t)/4];

    for (size_t i = 0; i < sizeof(indices)/sizeof(size_t); i += 4) {
        depths[i/4] = (quad){convert(vertices[indices[i]]), convert(vertices[indices[i+1]]), convert(vertices[indices[i+2]]), convert(vertices[indices[i+3]]), i/4};
    }
    dsortQuad(depths, sizeof(depths)/sizeof(quad));
    for (int i = 0; i < sizeof(depths)/sizeof(quad); i++) {
        drawQuad(depths[i]);
    }
}

void modelTri() {
    tri depths[sizeof(indices)/sizeof(size_t)/3];

    for (size_t i = 0; i < sizeof(indices)/sizeof(size_t); i += 3) {
        depths[i/3] = (tri){convert(vertices[indices[i]]), convert(vertices[indices[i+1]]), convert(vertices[indices[i+2]]), i/3};
    }
    dsortTri(depths, sizeof(depths)/sizeof(tri));
    for (int i = 0; i < sizeof(depths)/sizeof(tri); i++) {
        // if (i%10 != 1) { continue; }
        drawTri(depths[i]);
    }
}

void frame() {
    ClearBackground((Color){0,0,0,0}); //todo вынести в ./settings.h?
    // ClearBackground(BLACK);
   
    sina = sin(tc);
    cosa = cos(tc);

    switch (modelType) {
        case 3:
            modelTri();
            break;
        case 4:
            modelQuad();
            break;
    }

    // quad depths[sizeof(indices)/sizeof(size_t)/4];

    // for (size_t i = 0; i < sizeof(indices)/sizeof(size_t); i += 4) {
        /*if (depths.size == 0) {
            depths.size = sizeof(indices)/sizeof(size_t)/4;
            depths.values = (quad*)malloc(depths.size*sizeof(quad));
        }
        depths.values[i/4] = (quad){convert(vertices[indices[i]]), convert(vertices[indices[i+1]]), convert(vertices[indices[i+2]]), convert(vertices[indices[i+3]]), i/4};*/
        // depths[i/4] = (quad){convert(vertices[indices[i]]), convert(vertices[indices[i+1]]), convert(vertices[indices[i+2]]), convert(vertices[indices[i+3]]), i/4};
    // }
   
    // printf("[0]\n");
    // debugDepths();
    
    // depths = qdasort(&depths);
   
    // dsort(depths, sizeof(depths)/sizeof(quad));

    // printf("[1]\n");
    // debugDepths();


    // for (int i = 0; i < depths.size; i++) {
    // for (int i = 0; i < sizeof(depths)/sizeof(quad); i++) {
        // drawQuad(depths.values[i]);
        // drawQuad(depths[i]);
        // printf("%d", i);
    // }
    // printf("\n");

    if (!debug) {
        return;
    }
    char tstr[16];
    sprintf(tstr, "%.3f", tc);
    DrawText(tstr, 10, 10, 20, WHITE);
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        // printf("%s\n", argv[i]);
        if (strcmp(argv[i], "debug") == 0) { debug = true; }
        else if (strcmp(argv[i], "culling") == 0) { culling = true; }
        else if (strcmp(argv[i], "transparent") == 0) { transparent = true; }
        else if (strcmp(argv[i], "dist") == 0) {
            if (argc <= i+1) { printf("dist val?\n"); continue; }
            dist = atof(argv[i+1]);
        }
    }

    srand(time(NULL));

    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(((transparent) ? FLAG_WINDOW_TRANSPARENT : 0) | ((transparent) ? FLAG_WINDOW_UNDECORATED : 0) | FLAG_WINDOW_RESIZABLE | ((!transparent) ? FLAG_MSAA_4X_HINT : 0));
    
    InitWindow(dw, dh, "raylib");

    rlDisableBackfaceCulling(); //чтоб тебя

    SetTargetFPS(fps);

    // SetWindowOpacity(0.5);

    // GetWindowPosition()

    while (!WindowShouldClose()) {
        BeginDrawing();

        w = fmax(GetScreenWidth(), GetScreenHeight());
        h = w;
        // w = GetScreenWidth();
        // h = GetScreenHeight();

        frame();

        tc += dt;

        EndDrawing();

        // WaitTime(1000);
    }

    CloseWindow();

    // free(depths.values);

    return 0;
}
