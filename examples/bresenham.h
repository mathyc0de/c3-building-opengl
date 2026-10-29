#ifndef BRESENHAM_H
#define BRESENHAM_H

/*
 * Rasterização de segmento. As duas funções só produzem pixels inteiros;
 * quem chama decide o que fazer com cada pixel (aqui, um quadradinho 3D).
 * Nenhuma das duas usa GL_LINES: o algoritmo tem de ser nosso.
 */

typedef void (*PlotFn)(int x, int y, void *user);

void bresenham_line(int x0, int y0, int x1, int y1, PlotFn plot, void *user);
void dda_line(int x0, int y0, int x1, int y1, PlotFn plot, void *user);

#endif
