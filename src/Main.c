#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/Space.h"

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

typedef struct
{
    Vec3 position;
    Vec3 front;
    float yaw;
    float pitch;
    float fov;
    float speed;
    float sensitivity;
    float aspectRatio;
    bool locked;
} Camera;

Camera camera_create(float fov, float speed, float sensitivity, float aspectRatio);
void   camera_recomputeRotation(Camera* camera, GLFWwindow* window);
void   camera_updateAspectRatio(Camera* camera, float aspectRatio);

Vec3 window_getMovementVec(GLFWwindow* window);

const unsigned int WINDOW_WIDTH  = 800;
const unsigned int WINDOW_HEIGHT = 600;
const char*        WINDOW_TITLE  = "GL";

const float CAMERA_FOV            = 60.f;
const float CAMERA_INITIAL_ASPECT = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
const float CAMERA_SPEED          = 10.f;
const float CAMERA_SENSITIVITY    = 0.3f;

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

int currentWindowWidth  = WINDOW_WIDTH;
int currentWindowHeight = WINDOW_HEIGHT;

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

    bool lockPressed = false;
    float lastTime = 0.f;
    float deltaTime = 0.f;

    Camera camera = camera_create(
        CAMERA_FOV,
        CAMERA_SPEED,
        CAMERA_SENSITIVITY,
        CAMERA_INITIAL_ASPECT
    );

    while (!glfwWindowShouldClose(window))
    {
        shader_use(shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deltaTime = glfwGetTime() - lastTime;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            if (!lockPressed)
            {
                camera.locked = !camera.locked;
                lockPressed = true;
                glfwSetInputMode(
                    window,
                    GLFW_CURSOR,
                    camera.locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
                );
            }
        } else {
            lockPressed = false;
        }

        Vec3 front = vec3(0.f, 0.f, 0.f);
        front.x = cosf(radians(camera.yaw)) * cosf(radians(camera.pitch));
        front.y = sinf(radians(camera.pitch));
        front.z = sinf(radians(camera.yaw)) * cosf(radians(camera.pitch));
        camera.front = vec3_normalize(front);

        if (camera.locked)
        {
            Vec3 movementVec = vec3_normalize(window_getMovementVec(window));
            Vec3 movementDirection = vec3(0.f, 0.f, 0.f);
            Vec3 right = vec3_normalize(vec3_cross(camera.front, vec3(0.f, 1.f, 0.f)));
            
            movementDirection = vec3_add(movementDirection, vec3_scale(camera.front, movementVec.z));
            movementDirection = vec3_add(movementDirection, vec3_scale(right, movementVec.x));
            movementDirection = vec3_add(movementDirection, vec3_scale(vec3(0, 1.f, 0.f), movementVec.y));
            
            movementDirection = vec3_normalize(movementDirection);
            camera.position = vec3_add(camera.position, vec3_scale(movementDirection, CAMERA_SPEED * deltaTime));
            
            camera_recomputeRotation(&camera, window);
            glfwSetCursorPos(window, (double)currentWindowWidth / 2.f, (double)currentWindowHeight / 2.f);
        }

        Mat4* model = mat4(1.f);
        Mat4* view = mat4_lookAt(
            camera.position,
            vec3_add(camera.position, front),
            vec3(0.f, 1.f, 0.f)
        );
        Mat4* projection = mat4_perspective(
            60.f,
            (float)currentWindowWidth / currentWindowHeight,
            0.01f, 
            100.f
        );

        Mat4* mvp = mat4_multiply_many(3, view, model, projection);
        GLuint mvpLoc = glGetUniformLocation(shader, "mvp");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (const GLfloat*)(*mvp));

        vao_bind(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        vao_unbind(VAO);

        free(model);
        free(view);
        free(projection);
        free(mvp);

        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            camera.locked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL
        );

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

// Main Logic

Vec3 window_getMovementVec(GLFWwindow* window)
{
    Vec3 movementVec = vec3(0.f, 0.f, 0.f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movementVec.z += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movementVec.z -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movementVec.x -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movementVec.x += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        movementVec.y -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        movementVec.y += 1;
    }
    return movementVec;
}

// GL

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    currentWindowWidth = width;
    currentWindowHeight = height;
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

// Camera

Camera camera_create(float fov, float speed, float sensitivity, float aspectRatio)
{
    Camera camera =
    {
        .position = vec3(0.f, 0.f, 0.f),
        .front = vec3(0.f, 0.f, -1.f),
        .yaw = -90.f,
        .pitch = 0.f,
        .fov = fov,
        .speed = speed,
        .sensitivity = sensitivity,
        .aspectRatio = aspectRatio,
        .locked = false,
    };
    return camera;
}

void camera_recomputeRotation(Camera* camera, GLFWwindow* window)
{
    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    const float deltaX = (float)(mouseX - (float)windowWidth / 2.f) * CAMERA_SENSITIVITY;
    const float deltaY = (float)(mouseY - (float)windowHeight / 2.f) * CAMERA_SENSITIVITY;

    camera->yaw += deltaX * camera->sensitivity;
    camera->pitch -= deltaY * camera->sensitivity;

    if (camera->pitch > 89.f)
    {
        camera->pitch = 89.f;
    }
    else if (camera->pitch < -89.f)
    {
        camera->pitch = -89.f;
    }
    camera->yaw = fmod(camera->yaw, 360.f);
}

void camera_updateAspectRatio(Camera* camera, float aspectRatio)
{
    camera->aspectRatio = aspectRatio;
}
