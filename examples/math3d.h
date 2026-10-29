#ifndef MATH3D_H
#define MATH3D_H

/*
 * Matrizes 4x4 no layout column-major da OpenGL (o mesmo de glMultMatrixf).
 *
 * Num caderno a matriz aparece por linhas. Na memória, a OpenGL guarda
 * por colunas. O elemento da linha r e coluna c está em m[c * 4 + r]:
 *
 *   m[0]  m[4]  m[8]  m[12]
 *   m[1]  m[5]  m[9]  m[13]
 *   m[2]  m[6]  m[10] m[14]
 *   m[3]  m[7]  m[11] m[15]
 *
 * Cada função make_* preenche a matriz do zero. Nenhuma delas acumula
 * em cima de uma matriz anterior: a composição é mat4_multiply, ou uma
 * sequência de apply_matrix (glMultMatrixf).
 */

#define MATH3D_PI 3.14159265358979323846f

void mat4_identity(float m[16]);
void mat4_multiply(float out[16], const float a[16], const float b[16]);

void mat4_make_translation(float m[16], float tx, float ty, float tz);
void mat4_make_scale(float m[16], float sx, float sy, float sz);
void mat4_make_rotation_x(float m[16], float radians);
void mat4_make_rotation_y(float m[16], float radians);
void mat4_make_rotation_z(float m[16], float radians);

void mat4_make_perspective(float m[16], float fovy_rad, float aspect,
                           float znear, float zfar);
void mat4_make_ortho(float m[16], float left, float right, float bottom,
                     float top, float znear, float zfar);

void mat4_transform_point(const float m[16], float x, float y, float z,
                          float *ox, float *oy, float *oz);

/* Imprime em linhas matemáticas, para comparar com o caderno. */
void mat4_print(const char *name, const float m[16]);

/* Multiplica a matriz corrente (projeção ou modelview). */
void apply_matrix(const float m[16]);

#endif
