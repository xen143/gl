#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

const unsigned int WINDOW_WIDTH  = 400;
const unsigned int WINDOW_HEIGHT = 300;
const char*        WINDOW_TITLE  = "GL";

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return EXIT_FAILURE;
    }

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

    glClearColor(0.f, 0.5f, 0.5f, 1.f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
