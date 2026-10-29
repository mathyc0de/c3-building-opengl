#ifndef WORDMARK_H
#define WORDMARK_H

/*
 * A palavra "e-colab" em pixels. Cada segmento do glifo passa por
 * Bresenham ou DDA e vira um quadradinho no plano z = 0.
 */

void wordmark_draw(float center_x, float baseline_y, float pixel_size,
                   float r, float g, float b);

const char *wordmark_algorithm_name(void);
const char *wordmark_toggle_algorithm(void);

#endif
