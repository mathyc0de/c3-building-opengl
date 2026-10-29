#include "gl_platform.h"

#include "wordmark.h"
#include "bresenham.h"

#include <string.h>

#define WORDMARK_MAX_PIXELS 2048
#define WORDMARK_GRID_W 96
#define WORDMARK_GRID_H 24
#define WORDMARK_ADVANCE 7

typedef struct {
    const int (*seg)[4];
    int count;
} Glyph;

typedef struct {
    int xs[WORDMARK_MAX_PIXELS];
    int ys[WORDMARK_MAX_PIXELS];
    unsigned char seen[WORDMARK_GRID_H][WORDMARK_GRID_W];
    int count;
} PixelList;

/* Glifos numa grade 5 por 7. x cresce para a direita, y para cima. */
static const int seg_e[][4] = {
    {0, 6, 4, 6},
    {4, 6, 4, 3},
    {4, 3, 0, 3},
    {0, 6, 0, 0},
    {0, 0, 4, 0},
};

static const int seg_minus[][4] = {
    {0, 3, 4, 3},
};

static const int seg_c[][4] = {
    {4, 6, 0, 6},
    {0, 6, 0, 0},
    {0, 0, 4, 0},
};

static const int seg_o[][4] = {
    {0, 0, 4, 0},
    {4, 0, 4, 6},
    {4, 6, 0, 6},
    {0, 6, 0, 0},
};

static const int seg_l[][4] = {
    {0, 6, 0, 0},
    {0, 0, 4, 0},
};

static const int seg_a[][4] = {
    {4, 0, 4, 6},
    {0, 4, 4, 4},
    {0, 4, 0, 0},
    {0, 0, 4, 0},
    {0, 2, 4, 2},
};

static const int seg_b[][4] = {
    {0, 0, 0, 6},
    {0, 6, 4, 6},
    {4, 6, 4, 3},
    {4, 3, 0, 3},
    {0, 0, 4, 0},
    {4, 0, 4, 3},
};

#define GLYPH(arr) { arr, (int)(sizeof(arr) / sizeof((arr)[0])) }

static const Glyph kWord[] = {
    GLYPH(seg_e),
    GLYPH(seg_minus),
    GLYPH(seg_c),
    GLYPH(seg_o),
    GLYPH(seg_l),
    GLYPH(seg_a),
    GLYPH(seg_b),
};

static int g_use_dda = 0;

const char *wordmark_algorithm_name(void)
{
    return g_use_dda ? "DDA" : "Bresenham";
}

const char *wordmark_toggle_algorithm(void)
{
    g_use_dda = !g_use_dda;
    return wordmark_algorithm_name();
}

static void plot_store(int x, int y, void *user)
{
    PixelList *list = (PixelList *)user;

    if (x < 0 || y < 0 || x >= WORDMARK_GRID_W || y >= WORDMARK_GRID_H) {
        return;
    }
    if (list->seen[y][x]) {
        return;
    }
    if (list->count >= WORDMARK_MAX_PIXELS) {
        return;
    }

    list->seen[y][x] = 1;
    list->xs[list->count] = x;
    list->ys[list->count] = y;
    list->count++;
}

static void rasterize_word(PixelList *list)
{
    int glyph_index;
    int pen_x = 0;
    void (*line)(int, int, int, int, PlotFn, void *);

    memset(list, 0, sizeof(*list));
    line = g_use_dda ? dda_line : bresenham_line;

    for (glyph_index = 0; glyph_index < (int)(sizeof(kWord) / sizeof(kWord[0]));
         glyph_index++) {
        const Glyph *glyph = &kWord[glyph_index];
        int s;

        for (s = 0; s < glyph->count; s++) {
            line(pen_x + glyph->seg[s][0], glyph->seg[s][1],
                 pen_x + glyph->seg[s][2], glyph->seg[s][3],
                 plot_store, list);
        }
        pen_x += WORDMARK_ADVANCE;
    }
}

void wordmark_draw(float center_x, float baseline_y, float pixel_size,
                   float r, float g, float b)
{
    PixelList list;
    int max_x = 0;
    float ox;
    float cell;
    int i;

    rasterize_word(&list);
    if (list.count == 0) {
        return;
    }

    for (i = 0; i < list.count; i++) {
        if (list.xs[i] > max_x) {
            max_x = list.xs[i];
        }
    }

    ox = center_x - (float)(max_x + 1) * pixel_size * 0.5f;
    cell = pixel_size * 0.88f;

    glColor3f(r, g, b);
    glNormal3f(0.f, 0.f, 1.f);
    glBegin(GL_QUADS);
    for (i = 0; i < list.count; i++) {
        float x = ox + (float)list.xs[i] * pixel_size;
        float y = baseline_y + (float)list.ys[i] * pixel_size;

        glVertex3f(x, y, 0.f);
        glVertex3f(x + cell, y, 0.f);
        glVertex3f(x + cell, y + cell, 0.f);
        glVertex3f(x, y + cell, 0.f);
    }
    glEnd();
}
