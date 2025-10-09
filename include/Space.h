#ifndef SPACE_H
#define SPACE_H

#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

extern const float PI;

float radians(float degrees);
float degrees(float radians);

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef float Mat4[4][4];

Vec2  vec2(float x, float y);
float vec2_length(Vec2 vec);
Vec2  vec2_normalize(Vec2 vec);
void  vec2_normalize_inplace(Vec2* vec);
Vec2  vec2_add(Vec2 vecOne, Vec2 vecTwo);
void  vec2_add_inplace(Vec2* vecOne, Vec2 vecTwo);
Vec2  vec2_sub(Vec2 vecOne, Vec2 vecTwo);
void  vec2_sub_inplace(Vec2* vecOne, Vec2 vecTwo);
Vec2  vec2_scale(Vec2 vec, float scalar);
void  vec2_scale_inplace(Vec2* vec, float scalar);
Vec2  vec2_div(Vec2 vec, float divisor);
void  vec2_div_inplace(Vec2* vec, float divisor);
float vec2_dot(Vec2 vecOne, Vec2 vecTwo);
void  vec2_log(Vec2 vec);

Vec3  vec3(float x, float y, float z);
float vec3_length(Vec3 vec);
Vec3  vec3_normalize(Vec3 vec);
void  vec3_normalize_inplace(Vec3* vec);
Vec3  vec3_add(Vec3 vecOne, Vec3 vecTwo);
void  vec3_add_inplace(Vec3* vecOne, Vec3 vecTwo);
Vec3  vec3_sub(Vec3 vecOne, Vec3 vecTwo);
void  vec3_sub_inplace(Vec3* vecOne, Vec3 vecTwo);
Vec3  vec3_scale(Vec3 vec, float scalar);
void  vec3_scale_inplace(Vec3* vec, float scalar);
Vec3  vec3_div(Vec3 vec, float divisor);
void  vec3_div_inplace(Vec3* vec, float divisor);
float vec3_dot(Vec3 vecOne, Vec3 vecTwo);
Vec3  vec3_cross(Vec3 vecOne, Vec3 vecTwo);
void  vec3_log(Vec3 vec);

Mat4* mat4(float diagonalValue);
void  mat4_load_identity(Mat4* mat);
void  mat4_copy(Mat4* source, Mat4* target);
Mat4* mat4_scale(Mat4* mat, float scalar);
Mat4* mat4_multiply(Mat4* matOne, Mat4* matTwo);
void  mat4_multiply_inplace(Mat4* matOne, Mat4* matTwo);
Mat4* mat4_multiply_many(int count, ...);
void  mat4_multiply_many_inplace(Mat4* mat, int count, ...);
Mat4* mat4_translate(Mat4* mat, Vec3 vec);
void  mat4_translate_inplace(Mat4* mat, Vec3 vec);
Mat4* mat4_rotate(Mat4* mat, float degrees, Vec3 rotationVec);
void  mat4_rotate_inplace(Mat4* mat, float degrees, Vec3 rotationVec);
Mat4* mat4_ortho(float left, float right, float bottom, float top, float zNear, float zFar);
void  mat4_ortho_inplace(Mat4* mat, float left, float right, float bottom, float top, float zNear, float zFar);
Mat4* mat4_perspective(float fov, float aspect, float zNear, float zFar);
void  mat4_perspective_inplace(Mat4* mat, float fov, float aspect, float zNear, float zFar);
Mat4* mat4_lookAt(Vec3 eye, Vec3 target, Vec3 up);
void  mat4_lookAt_inplace(Mat4* mat, Vec3 eye, Vec3 target, Vec3 up);
void  mat4_log(Mat4* mat);

#endif // SPACE_H
