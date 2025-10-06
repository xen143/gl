#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>

const float PI = 3.141593f;
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
Vec2  vec2_add(Vec2 vecOne, Vec2 vecTwo);
Vec2  vec2_sub(Vec2 vecOne, Vec2 vecTwo);
Vec2  vec2_scale(Vec2 vec, float scalar);
Vec2  vec2_div(Vec2 vec, float divisor);
float vec2_dot(Vec2 vecOne, Vec2 vecTwo);
void  vec2_log(Vec2 vec);

Vec3  vec3(float x, float y, float z);
float vec3_length(Vec3 vec);
Vec3  vec3_normalize(Vec3 vec);
Vec3  vec3_add(Vec3 vecOne, Vec3 vecTwo);
Vec3  vec3_sub(Vec3 vecOne, Vec3 vecTwo);
Vec3  vec3_scale(Vec3 vec, float scalar);
Vec3  vec3_div(Vec3 vec, float divisor);
float vec3_dot(Vec3 vecOne, Vec3 vecTwo);
Vec3  vec3_cross(Vec3 vecOne, Vec3 vecTwo);
void  vec3_log(Vec3 vec);

Mat4* mat4(float diagonalValue);
void  mat4_copy(Mat4* source, Mat4* target);
Mat4* mat4_scale(Mat4* mat, float scalar);
Mat4* mat4_multiply(Mat4* matOne, Mat4* matTwo);
void  mat4_multiply_inplace(Mat4* matOne, Mat4* matTwo);
Mat4* mat4_multiply_many(int count, ...);
Mat4* mat4_translate(Mat4* mat, Vec3 vec);
void  mat4_translate_inplace(Mat4* mat, Vec3 vec);
Mat4* mat4_rotate(Mat4* mat, float degrees, Vec3 rotationVec);
void  mat4_rotate_inplace(Mat4* mat, float degrees, Vec3 rotationVec);
Mat4* mat4_ortho(float left, float right, float bottom, float top, float zNear, float zFar);
Mat4* mat4_perspective(float fov, float aspect, float zNear, float zFar);
void  mat4_log(Mat4* mat);

typedef GLuint Shader;
typedef GLuint VAO;
typedef GLuint VBO; 
typedef GLuint EBO;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Shader shader_create(const char* vertexShaderSource, const char* fragmentShaderSource);
void   shader_use(Shader ID);
void   shader_delete(Shader ID);

VAO  vao_create();
void vao_linkAttrib(VBO VBO, GLuint index, GLuint size, GLenum type, GLsizei stride, const void* offset);
void vao_bind(VAO VAO);
void vao_unbind(VAO VAO);
void vao_delete(VAO* VAO);

VBO  vbo_create(const GLfloat* vertices, GLsizeiptr verticesSize);
void vbo_bind(VBO VBO);
void vbo_unbind(VBO VBO);
void vbo_delete(VBO* VBO);

EBO  ebo_create(const GLuint* indices, GLsizeiptr indicesSize);
void ebo_bind(EBO EBO);
void ebo_unbind(EBO EBO);
void ebo_delete(EBO* EBO);

const unsigned int WINDOW_WIDTH  = 400;
const unsigned int WINDOW_HEIGHT = 300;
const char*        WINDOW_TITLE  = "GL";

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec3 vertCol;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "  vertCol = aCol;\n"
    "  gl_Position = mvp * vec4(aPos, 1.f);\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 vertCol;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(vertCol, 1.f);\n"
    "}\0";

const GLfloat vertices[] =
{
    -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f,
     0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f,
     0.5f,  0.5f,  0.5f, 1.f, 1.f, 0.f,
    -0.5f, -0.5f, -0.5f, 1.f, 0.f, 1.f,
     0.5f, -0.5f, -0.5f, 0.f, 1.f, 1.f,
    -0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f,
     0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f,
};
const GLuint indices[] =
{
    0, 1, 3, // Front
    0, 3, 2, // Front
    1, 5, 7, // Right
    1, 7, 3, // Right
    5, 4, 6, // Back
    5, 6, 7, // Back
    4, 0, 2, // Left
    4, 2, 6, // Left
    2, 3, 7, // Top
    2, 7, 6, // Top
    5, 4, 0, // Bottom
    5, 0, 1, // Bottom
};

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE,
        NULL,
        NULL
    );
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to initialize a window!\n");
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW!\n");
        fprintf(stderr, "Error:\n%s\n", glewGetErrorString(glewStatus));
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.5f, 0.5f, 1.f);

    Shader shader = shader_create(vertexShaderSource, fragmentShaderSource);

    VAO VAO = vao_create();
    VBO VBO = vbo_create(vertices, sizeof(vertices));
    EBO EBO = ebo_create(indices, sizeof(indices));

    vao_bind(VAO);
    vbo_bind(VBO);
    ebo_bind(EBO);

    vao_linkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(0));
    vao_linkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    vao_unbind(VAO);
    vbo_unbind(VBO);
    ebo_unbind(EBO);

    float lastTime = 0.f;
    float deltaTime = 0.f;
    float rotation = 0.f;

    while (!glfwWindowShouldClose(window))
    {
        shader_use(shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deltaTime = glfwGetTime() - lastTime;
        rotation += 25.f * deltaTime;

        GLuint mvpLoc = glGetUniformLocation(shader, "mvp");
        Mat4* model = mat4(1.f);
        mat4_rotate_inplace(model, rotation, vec3(1.f, 1.f, 1.f));
        Mat4* view = mat4(1.f);
        mat4_translate_inplace(view, vec3(0.0f, 0.f, -3.0f));
        Mat4* projection = mat4_perspective(60.f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 100.f);
        Mat4* mvp = mat4_multiply_many(3, model, view, projection);
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (const GLfloat*)(*mvp));

        vao_bind(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        vao_unbind(VAO);

        free(model);
        free(view);
        free(projection);
        free(mvp);

        lastTime = glfwGetTime();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao_delete(&VAO);
    vbo_delete(&VBO);
    ebo_delete(&EBO);

    shader_delete(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Math

float radians(float degrees)
{
    return degrees * PI / 180.f;
}

float degrees(float radians)
{
    return radians * 180.f / PI;
}

// Vec2

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

Vec2 vec2_add(Vec2 vecOne, Vec2 vecTwo)
{
    Vec2 result =
    {
        .x = vecOne.x + vecTwo.x,
        .y = vecOne.y + vecTwo.y,
    };
    return result;
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

Vec2 vec2_scale(Vec2 vec, float scalar)
{
    Vec2 result =
    {
        .x = vec.x * scalar,
        .y = vec.y * scalar,
    };
    return result;
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

float vec2_dot(Vec2 vecOne, Vec2 vecTwo)
{
    return vecOne.x * vecTwo.x + vecOne.y * vecTwo.y;
}

void vec2_log(Vec2 vec)
{
    printf("X: %.2f; Y: %.2f\n", vec.x, vec.y);
}

// Vec3

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

// Mat4

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
}

Mat4* mat4_ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    Mat4* result = mat4(1.f);
    (*result)[0][0] =  2.f / (right - left);
    (*result)[1][1] =  2.f / (top - bottom);
    (*result)[2][2] = -2.f / (zFar - zNear);
    (*result)[3][0] = -(right + left) / (right - left);
    (*result)[3][1] = -(top + bottom) / (top - bottom);
    (*result)[3][2] = -(zFar + zNear) / (zFar - zNear);
    return result;
}

Mat4* mat4_perspective(float fov, float aspect, float zNear, float zFar)
{
    Mat4* result = mat4(0.f);
    float halfTanFov = tanf(radians(fov) / 2.f);
    (*result)[0][0] = 1.f / (halfTanFov * aspect);
    (*result)[1][1] = 1.f / halfTanFov;
    (*result)[2][2] = (zFar + zNear) / (zNear - zFar);
    (*result)[2][3] = -1.f;
    (*result)[3][2] = -(2.f * zNear * zFar) / (zFar - zNear);
    return result;
}

// GL

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

// Shader

Shader shader_create(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    char infoLog[512];
    int success = 0;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile the vertex shader!\n");
        fprintf(stderr, "Error:\n%s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile the fragment shader!\n");
        fprintf(stderr, "Error:\n%s\n", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Failed to link the shader program!\n");
        fprintf(stderr, "Error:\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void shader_use(Shader ID)
{
    glUseProgram(ID);
}

void shader_delete(Shader ID)
{
    glDeleteProgram(ID);
}

// VAO

VAO vao_create()
{
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    return VAO;
}

void vao_linkAttrib(VBO VBO, GLuint index, GLuint size, GLenum type, GLsizei stride, const void* offset)
{
    vbo_bind(VBO);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
    vbo_unbind(VBO);
}

void vao_bind(VAO VAO)
{
    glBindVertexArray(VAO);
}

void vao_unbind(VAO VAO)
{
    (void)VAO;
    glBindVertexArray(0);
}

void vao_delete(VAO* VAO)
{
    glDeleteVertexArrays(1, VAO);
}
// VBO

VBO vbo_create(const GLfloat* vertices, GLsizeiptr verticesSize)
{
    GLuint VBO;
    glGenBuffers(1, &VBO);
    vbo_bind(VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
    vbo_unbind(VBO);
    return VBO;
}

void vbo_bind(VBO VBO)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void vbo_unbind(VBO VBO)
{
    (void)VBO;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(VBO* VBO)
{
    glDeleteBuffers(1, VBO);
}

// EBO

EBO ebo_create(const GLuint* indices, GLsizeiptr indicesSize)
{
    GLuint EBO;
    glGenBuffers(1, &EBO);
    ebo_bind(EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    ebo_unbind(EBO);
    return EBO;
}

void ebo_bind(EBO EBO)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void ebo_unbind(EBO EBO)
{
    (void)EBO;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_delete(EBO* EBO)
{
    glDeleteBuffers(1, EBO);
}
