#ifndef GL_PLATFORM_H
#define GL_PLATFORM_H

/*
 * Inclua este arquivo antes de qualquer outro cabeçalho OpenGL.
 * No Windows, windows.h define os macros near e far. Se um parâmetro
 * se chamar near ou far, a compilação quebra. Os nossos parâmetros
 * de projeção se chamam znear e zfar de propósito.
 */

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <GL/glut.h>

#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif

#endif
