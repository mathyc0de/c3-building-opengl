#include "gl_platform.h"

#include "petal.h"
#include "math3d.h"

#include <math.h>

/*
 * Ajustem estes números com docs/referencia-ecolab.jpg aberto do lado.
 * Mais anéis deixam o traço mais parecido com a referência; se os anéis
 * encostarem uns nos outros, diminuam kTubeRadius.
 */
#define PETAL_RINGS 4
#define PETAL_SEGMENTS 72
#define PETAL_SLICES 8
#define PETAL_TUBE_RADIUS 0.014f

static void ellipse_frame(float t, float rx, float ry,
                           float *px, float *py, float *nx, float *ny)
{
    float tx = -rx * sinf(t);
    float ty = ry * cosf(t);
    float len = sqrtf(tx * tx + ty * ty);

    *px = rx * cosf(t);
    *py = ry * sinf(t);

    tx /= len;
    ty /= len;
    /* Tangente anti-horária, girada 90° no sentido horário: aponta para fora. */
    *nx = ty;
    *ny = -tx;
}

static void draw_tube(float rx, float ry, float radius)
{
    int slice;

    for (slice = 0; slice < PETAL_SLICES; slice++) {
        float a0 = (2.f * MATH3D_PI * (float)slice) / (float)PETAL_SLICES;
        float a1 = (2.f * MATH3D_PI * (float)(slice + 1)) / (float)PETAL_SLICES;
        float c0 = cosf(a0);
        float s0 = sinf(a0);
        float c1 = cosf(a1);
        float s1 = sinf(a1);
        int i;

        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i <= PETAL_SEGMENTS; i++) {
            float t = (2.f * MATH3D_PI * (float)i) / (float)PETAL_SEGMENTS;
            float cx;
            float cy;
            float nx;
            float ny;
            int k;

            ellipse_frame(t, rx, ry, &cx, &cy, &nx, &ny);

            for (k = 0; k < 2; k++) {
                float ca = (k == 0) ? c0 : c1;
                float sa = (k == 0) ? s0 : s1;

                glNormal3f(ca * nx, ca * ny, sa);
                glVertex3f(cx + radius * ca * nx,
                           cy + radius * ca * ny,
                           radius * sa);
            }
        }
        glEnd();
    }
}

void petal_draw(void)
{
    int ring;

    for (ring = 0; ring < PETAL_RINGS; ring++) {
        float u = (float)ring / (float)(PETAL_RINGS - 1);
        float rx = 0.12f + u * 0.20f;
        float ry = 0.22f + u * 0.28f;

        draw_tube(rx, ry, PETAL_TUBE_RADIUS);
    }
}
