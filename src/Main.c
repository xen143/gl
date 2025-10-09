#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/Window.h"
#include "../include/Camera.h"
#include "../include/Graphics.h"
#include "../include/Space.h"

const unsigned int WINDOW_WIDTH  = 800;
const unsigned int WINDOW_HEIGHT = 600;
const char*        WINDOW_TITLE  = "GL";

const float CAMERA_FOV            = 60.f;
const float CAMERA_INITIAL_ASPECT = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
const float CAMERA_SPEED          = 10.f;
const float CAMERA_SENSITIVITY    = 0.25f;

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec3 vertCol;\n"
    "uniform mat4 cameraMatrix;\n"
    "uniform mat4 modelMatrix;\n"
    "void main()\n"
    "{\n"
    "  vertCol = aCol;\n"
    "  gl_Position = cameraMatrix * modelMatrix * vec4(aPos, 1.f);\n"
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

    Window window = window_create(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE
    );

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

    Camera camera = camera_create(
        CAMERA_FOV,
        CAMERA_SPEED,
        CAMERA_SENSITIVITY,
        CAMERA_INITIAL_ASPECT
    );

    Mat4 model;
    mat4_load_identity(&model);

    while (!window_shouldClose(&window))
    {
        shader_use(shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window_updateDeltaTime(&window);

        if (window_isKeyPressed(&window, GLFW_KEY_E))
        {
            if (!lockPressed)
            {
                lockPressed = true;
                camera.locked
                    ? camera_unlock(&camera)
                    : camera_lock(&camera);
                camera.locked
                    ? window_hideCursor(&window)
                    : window_showCursor(&window);
            }
        } else {
            lockPressed = false;
        }

        if (camera.locked)
        {
            camera_recomputePosition(&camera, &window);
            camera_recomputeRotation(&camera, &window);
            window_centerCursor(&window);
        }

        camera_recomputeMatrix(&camera);
        shader_setMat4(shader, "cameraMatrix", &camera.matrix);
        shader_setMat4(shader, "modelMatrix", &model);

        vao_bind(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        vao_unbind(VAO);

        window_swapBuffers(&window);
        glfwPollEvents();
    }

    vao_delete(&VAO);
    vbo_delete(&VBO);
    ebo_delete(&EBO);

    shader_delete(shader);
    window_destroy(&window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
