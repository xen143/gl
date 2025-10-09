#include "../include/Space.h"

const float PI = 3.141593f;

float radians(float degrees)
{
    return degrees * PI / 180.f;
}

float degrees(float radians)
{
    return radians * 180.f / PI;
}

Vec2 vec2(float x, float y)
{
    Vec2 vec =
    {
        .x = x,
        .y = y,
    };
    return vec;
}

float vec2_length(Vec2 vec)
{
    return (float)(sqrt(pow(vec.x, 2) + pow(vec.y, 2)));
}

Vec2 vec2_normalize(Vec2 vec)
{
    float length = vec2_length(vec);
    if (length == 0.f)
    {
        return vec2(0.f, 0.f);
    }
    Vec2 normalizedVec =
    {
        .x = vec.x / length,
        .y = vec.y / length,
    };
    return normalizedVec;
}

void vec2_normalize_inplace(Vec2* vec)
{
    float length = vec2_length(*vec);
    if (length == 0.f)
    {
        vec->x = 0.f;
        vec->y = 0.f;
    }
    vec->x /= length;
    vec->y /= length;
}

Vec2 vec2_add(Vec2 vecOne, Vec2 vecTwo)
{
    Vec2 result =
    {
        .x = vecOne.x + vecTwo.x,
        .y = vecOne.y + vecTwo.y,
    };
    return result;
}

void vec2_add_inplace(Vec2* vecOne, Vec2 vecTwo)
{
    vecOne->x += vecTwo.x;
    vecOne->y += vecTwo.y;
}

Vec2 vec2_sub(Vec2 vecOne, Vec2 vecTwo)
{
    Vec2 result =
    {
        .x = vecOne.x - vecTwo.x,
        .y = vecOne.y - vecTwo.y,
    };
    return result;
}

void vec2_sub_inplace(Vec2* vecOne, Vec2 vecTwo)
{
    vecOne->x -= vecTwo.x;
    vecOne->y -= vecTwo.y;
}

Vec2 vec2_scale(Vec2 vec, float scalar)
{
    Vec2 result =
    {
        .x = vec.x * scalar,
        .y = vec.y * scalar,
    };
    return result;
}

void vec2_scale_inplace(Vec2* vec, float scalar)
{
    vec->x *= scalar;
    vec->y *= scalar;
}

Vec2 vec2_div(Vec2 vec, float divisor)
{
    if (divisor == 0.f)
    {
        return vec2(0.f, 0.f);
    }
    Vec2 result =
    {
        .x = vec.x / divisor,
        .y = vec.y / divisor,
    };
    return result;
}

void vec2_div_inplace(Vec2* vec, float divisor)
{
    if (divisor == 0.f)
    {
        vec->x = 0.f;
        vec->y = 0.f;
    }
    vec->x /= divisor;
    vec->y /= divisor;
}

float vec2_dot(Vec2 vecOne, Vec2 vecTwo)
{
    return vecOne.x * vecTwo.x + vecOne.y * vecTwo.y;
}

void vec2_log(Vec2 vec)
{
    printf("X: %.2f; Y: %.2f\n", vec.x, vec.y);
}

Vec3 vec3(float x, float y, float z)
{
    Vec3 vec =
    {
        .x = x,
        .y = y,
        .z = z,
    };
    return vec;
}

float vec3_length(Vec3 vec)
{
    return (float)(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
}

Vec3 vec3_normalize(Vec3 vec)
{
    float length = vec3_length(vec);
    if (length == 0.f)
    {
        return vec3(0.f, 0.f, 0.f);
    }
    Vec3 normalizedVec =
    {
        .x = vec.x / length,
        .y = vec.y / length,
        .z = vec.z / length,
    };
    return normalizedVec;
}

void vec3_normalize_inplace(Vec3* vec)
{
    float length = vec3_length(*vec);
    if (length == 0.f)
    {
        vec->x = 0.f;
        vec->y = 0.f;
        vec->z = 0.f;
    }
    vec->x /= length;
    vec->y /= length;
    vec->z /= length;
}

Vec3 vec3_add(Vec3 vecOne, Vec3 vecTwo)
{
    Vec3 result =
    {
        .x = vecOne.x + vecTwo.x,
        .y = vecOne.y + vecTwo.y,
        .z = vecOne.z + vecTwo.z,
    };
    return result;
}

void vec3_add_inplace(Vec3* vecOne, Vec3 vecTwo)
{
    vecOne->x += vecTwo.x;
    vecOne->y += vecTwo.y;
    vecOne->z += vecTwo.z;
}

Vec3 vec3_sub(Vec3 vecOne, Vec3 vecTwo)
{
    Vec3 result =
    {
        .x = vecOne.x - vecTwo.x,
        .y = vecOne.y - vecTwo.y,
        .z = vecOne.z - vecTwo.z,
    };
    return result;
}

void vec3_sub_inplace(Vec3* vecOne, Vec3 vecTwo)
{
    vecOne->x -= vecTwo.x;
    vecOne->y -= vecTwo.y;
    vecOne->z -= vecTwo.z;
}

Vec3 vec3_scale(Vec3 vec, float scalar)
{
    Vec3 result =
    {
        .x = vec.x * scalar,
        .y = vec.y * scalar,
        .z = vec.z * scalar,
    };
    return result;
}

void vec3_scale_inplace(Vec3* vec, float scalar)
{
    vec->x *= scalar;
    vec->y *= scalar;
    vec->z *= scalar;
}

Vec3 vec3_div(Vec3 vec, float divisor)
{
    if (divisor == 0.f)
    {
        return vec3(0.f, 0.f, 0.f);
    }
    Vec3 result =
    {
        .x = vec.x / divisor,
        .y = vec.y / divisor,
        .z = vec.z / divisor,
    };
    return result;
}

void vec3_div_inplace(Vec3* vec, float divisor)
{
    if (divisor == 0.f)
    {
        vec->x = 0.f;
        vec->y = 0.f;
        vec->z = 0.f;
    }
    vec->x /= divisor;
    vec->y /= divisor;
    vec->z /= divisor;
}

float vec3_dot(Vec3 vecOne, Vec3 vecTwo)
{
    return vecOne.x * vecTwo.x + vecOne.y * vecTwo.y + vecOne.z * vecTwo.z;
}

Vec3 vec3_cross(Vec3 vecOne, Vec3 vecTwo)
{
    Vec3 result =vec3(
        (vecOne.y * vecTwo.z) - (vecOne.z * vecTwo.y),
        (vecOne.z * vecTwo.x) - (vecOne.x * vecTwo.z),
        (vecOne.x * vecTwo.y) - (vecOne.y * vecTwo.x)
    );
    return result;
}

void vec3_log(Vec3 vec)
{
    printf("X: %.2f; Y: %.2f; Z: %.2f\n", vec.x, vec.y, vec.z);
}

Mat4* mat4(float diagonalValue)
{
    Mat4* mat = malloc(sizeof(Mat4));
    if (mat == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            (*mat)[i][j] = (i == j) ? diagonalValue : 0.f;
    return mat;
}

void mat4_load_identity(Mat4* mat)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            (*mat)[i][j] = (i == j) ? 1.f : 0.f;
}

void mat4_copy(Mat4* source, Mat4* target)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            (*target)[i][j] = (*source)[i][j];
}

void mat4_log(Mat4* mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%.2f ", (*mat)[i][j]);
        }
        printf("\n");
    }
}

Mat4* mat4_scale(Mat4* mat, float scalar)
{
    Mat4* result = malloc(sizeof(Mat4));
    if (result == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            (*result)[i][j] = (*mat)[i][j] * scalar;
    return result;
}

Mat4* mat4_multiply(Mat4* matOne, Mat4* matTwo)
{
    Mat4* result = mat4(0.f);
    if (result == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                (*result)[i][j] += (*matOne)[i][k] * (*matTwo)[k][j];
    return result;
}

void mat4_multiply_inplace(Mat4* matOne, Mat4* matTwo)
{
    Mat4* temp = mat4(0.f);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                (*temp)[i][j] += (*matOne)[i][k] * (*matTwo)[k][j];
    mat4_copy(temp, matOne);
    free(temp);
}

Mat4* mat4_multiply_many(int count, ...)
{
    if (count < 1)
        return NULL;

    va_list args;
    va_start(args, count);

    Mat4* result = mat4(1.f);
    Mat4* first = va_arg(args, Mat4*);

    if (result == NULL || first == NULL)
    {
        va_end(args);
        return NULL;
    }

    mat4_copy(first, result);

    for (int i = 1; i < count; i++)
    {
        Mat4* next = va_arg(args, Mat4*);
        Mat4* temp = mat4_multiply(result, next);
        free(result);
        result = temp;
    }

    va_end(args);
    return result;
}

void mat4_multiply_many_inplace(Mat4* mat, int count, ...)
{
    if (count < 1 || mat == NULL)
        return;

    va_list args;
    va_start(args, count);

    Mat4* matrices[count];
    for (int i = 0; i < count; i++)
    {
        matrices[i] = va_arg(args, Mat4*);
    }

    Mat4 tempResult;
    mat4_copy(matrices[count - 1], &tempResult);

    for (int i = count - 2; i >= 0; i--)
    {
        if (matrices[i] != NULL)
        {
            Mat4* newResult = mat4_multiply(matrices[i], &tempResult);
            if (newResult != NULL)
            {
                mat4_copy(newResult, &tempResult);
                free(newResult);
            }
        }
    }

    mat4_copy(&tempResult, mat);
    va_end(args);
}

Mat4* mat4_translate(Mat4* mat, Vec3 vec)
{
    Mat4* result = malloc(sizeof(Mat4));
    if (result == NULL)
    {
        return NULL;
    }
    mat4_copy(mat, result);
    (*result)[3][0] += vec.x;
    (*result)[3][1] += vec.y;
    (*result)[3][2] += vec.z;
    return result;
}

void mat4_translate_inplace(Mat4* mat, Vec3 vec)
{
    (*mat)[3][0] += vec.x;
    (*mat)[3][1] += vec.y;
    (*mat)[3][2] += vec.z;
}

Mat4* mat4_rotate(Mat4* mat, float degrees, Vec3 rotationVec)
{
    Mat4* result = mat4(0.f);
    Vec3 normalizedRotationVec = vec3_normalize(rotationVec);
    mat4_copy(mat, result);

    const float sinTheta = sinf(radians(degrees));
    const float cosTheta = cosf(radians(degrees));
    const float oneMinusCosTheta = 1.f - cosTheta;

    (*result)[0][0] = cosTheta + normalizedRotationVec.x * normalizedRotationVec.x * oneMinusCosTheta;
    (*result)[0][1] = normalizedRotationVec.x * normalizedRotationVec.y * oneMinusCosTheta - normalizedRotationVec.z * sinTheta;
    (*result)[0][2] = normalizedRotationVec.x * normalizedRotationVec.z * oneMinusCosTheta + normalizedRotationVec.y * sinTheta;
    (*result)[1][0] = normalizedRotationVec.y * normalizedRotationVec.x * oneMinusCosTheta + normalizedRotationVec.z * sinTheta;
    (*result)[1][1] = cosTheta + normalizedRotationVec.y * normalizedRotationVec.y * oneMinusCosTheta;
    (*result)[1][2] = normalizedRotationVec.y * normalizedRotationVec.z * oneMinusCosTheta - normalizedRotationVec.x * sinTheta;
    (*result)[2][0] = normalizedRotationVec.z * normalizedRotationVec.x * oneMinusCosTheta - normalizedRotationVec.y * sinTheta;
    (*result)[2][1] = normalizedRotationVec.z * normalizedRotationVec.y * oneMinusCosTheta + normalizedRotationVec.x * sinTheta;
    (*result)[2][2] = cosTheta + normalizedRotationVec.z * normalizedRotationVec.z * oneMinusCosTheta;

    mat4_multiply_inplace(result, mat);
    return result;
}

void mat4_rotate_inplace(Mat4* mat, float degrees, Vec3 rotationVec)
{
    Mat4* rotationMat = mat4(0.f);
    Vec3 normalizedRotationVec = vec3_normalize(rotationVec);
    mat4_copy(mat, rotationMat);

    const float sinTheta = sinf(radians(degrees));
    const float cosTheta = cosf(radians(degrees));
    const float oneMinusCosTheta = 1.f - cosTheta;

    (*rotationMat)[0][0] = cosTheta + normalizedRotationVec.x * normalizedRotationVec.x * oneMinusCosTheta;
    (*rotationMat)[0][1] = normalizedRotationVec.x * normalizedRotationVec.y * oneMinusCosTheta - normalizedRotationVec.z * sinTheta;
    (*rotationMat)[0][2] = normalizedRotationVec.x * normalizedRotationVec.z * oneMinusCosTheta + normalizedRotationVec.y * sinTheta;
    (*rotationMat)[1][0] = normalizedRotationVec.y * normalizedRotationVec.x * oneMinusCosTheta + normalizedRotationVec.z * sinTheta;
    (*rotationMat)[1][1] = cosTheta + normalizedRotationVec.y * normalizedRotationVec.y * oneMinusCosTheta;
    (*rotationMat)[1][2] = normalizedRotationVec.y * normalizedRotationVec.z * oneMinusCosTheta - normalizedRotationVec.x * sinTheta;
    (*rotationMat)[2][0] = normalizedRotationVec.z * normalizedRotationVec.x * oneMinusCosTheta - normalizedRotationVec.y * sinTheta;
    (*rotationMat)[2][1] = normalizedRotationVec.z * normalizedRotationVec.y * oneMinusCosTheta + normalizedRotationVec.x * sinTheta;
    (*rotationMat)[2][2] = cosTheta + normalizedRotationVec.z * normalizedRotationVec.z * oneMinusCosTheta;

    mat4_multiply_inplace(mat, rotationMat);
    free(rotationMat);
}

Mat4* mat4_ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    Mat4* result = mat4(1.f);
    mat4_ortho_inplace(result, left, right, bottom, top, zNear, zFar);
    return result;
}

void mat4_ortho_inplace(Mat4* mat, float left, float right, float bottom, float top, float zNear, float zFar)
{
    (*mat)[0][0] =  2.f / (right - left);
    (*mat)[1][1] =  2.f / (top - bottom);
    (*mat)[2][2] = -2.f / (zFar - zNear);
    (*mat)[3][0] = -(right + left) / (right - left);
    (*mat)[3][1] = -(top + bottom) / (top - bottom);
    (*mat)[3][2] = -(zFar + zNear) / (zFar - zNear);
    (*mat)[3][3] = 1.f;
}

Mat4* mat4_perspective(float fov, float aspect, float zNear, float zFar)
{
    Mat4* result = mat4(0.f);
    mat4_perspective_inplace(result, fov, aspect, zNear, zFar);
    return result;
}

void mat4_perspective_inplace(Mat4* mat, float fov, float aspect, float zNear, float zFar)
{
    float halfTanFov = tanf(radians(fov) / 2.f);
    (*mat)[0][0] = 1.f / (halfTanFov * aspect);
    (*mat)[1][1] = 1.f / halfTanFov;
    (*mat)[2][2] = (zFar + zNear) / (zNear - zFar);
    (*mat)[2][3] = -1.f;
    (*mat)[3][2] = -(2.f * zNear * zFar) / (zFar - zNear);
    (*mat)[3][3] = 0.f;
}

Mat4* mat4_lookAt(Vec3 eye, Vec3 target, Vec3 up)
{
    Mat4* result = mat4(0.f);
    mat4_lookAt_inplace(result, eye, target, up);
    return result;
}

void mat4_lookAt_inplace(Mat4* mat, Vec3 eye, Vec3 target, Vec3 up)
{
    Vec3 front = vec3_normalize(vec3_sub(eye, target));
    Vec3 right = vec3_normalize(vec3_cross(up, front));
    Vec3 newUp = vec3_cross(front, right);
    mat4_load_identity(mat);

    (*mat)[0][0] = right.x;
    (*mat)[1][0] = right.y;
    (*mat)[2][0] = right.z;
    (*mat)[0][1] = newUp.x;
    (*mat)[1][1] = newUp.y;
    (*mat)[2][1] = newUp.z;
    (*mat)[0][2] = front.x;
    (*mat)[1][2] = front.y;
    (*mat)[2][2] = front.z;
    (*mat)[3][0] = -vec3_dot(right, eye);
    (*mat)[3][1] = -vec3_dot(newUp, eye);
    (*mat)[3][2] = -vec3_dot(front, eye);
    (*mat)[3][3] = 1.f;
}
