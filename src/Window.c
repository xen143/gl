#include "../include/Window.h"

static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Window* window_create(int width, int height, const char* title)
{
    Window* window = malloc(sizeof(Window));
    *window = (Window)
    {
        .glfwWindow = NULL,
        .resizeCallback = NULL,
        .width = width,
        .height = height,
        .deltaTime = 0.f,
        ._lastTime = 0.f,
    };
    GLFWwindow* glfwWindow = glfwCreateWindow(
        width,
        height,
        title,
        NULL,
        NULL
    );
    if (glfwWindow == NULL)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to initialize a window!\n");
        exit(EXIT_FAILURE);
    }
    window->glfwWindow = glfwWindow;

    glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
    glfwSetWindowUserPointer(glfwWindow, window);
    glfwMakeContextCurrent(glfwWindow);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW!\n");
        fprintf(stderr, "Error:\n%s\n", glewGetErrorString(glewStatus));
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.0f, 0.0f, 1.f);
    glViewport(0, 0, width, height);

    return window;
}

void window_swapBuffers(Window* window)
{
    glfwSwapBuffers(window->glfwWindow);
}

void window_updateDeltaTime(Window* window)
{
    double currentTime = glfwGetTime();
    window->deltaTime = currentTime - window->_lastTime;
    window->_lastTime = currentTime;
}

void window_destroy(Window* window)
{
    glfwDestroyWindow(window->glfwWindow);
    window->glfwWindow = NULL;
    window->width = 0;
    window->height = 0;
    free(window);
}

bool window_shouldClose(Window* window)
{
    return glfwWindowShouldClose(window->glfwWindow);
}

bool window_isKeyPressed(Window* window, int key)
{
    return glfwGetKey(window->glfwWindow, key) == GLFW_PRESS;
}

void window_showCursor(Window* window)
{
    glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void window_hideCursor(Window* window)
{
    glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void window_centerCursor(Window* window)
{
    glfwSetCursorPos(window->glfwWindow, (double)window->width / 2.f, (double)window->height / 2.f);
}

Vec3 window_getMovementVec(Window* window)
{
    Vec3 movementVec = vec3(0.f, 0.f, 0.f);
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        movementVec.z += 1;
    }
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        movementVec.z -= 1;
    }
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        movementVec.x -= 1;
    }
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        movementVec.x += 1;
    }
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        movementVec.y -= 1;
    }
    if (glfwGetKey(window->glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        movementVec.y += 1;
    }
    return movementVec;
}

void window_setResizeCallback(Window* window, WindowResizeCallback callback)
{
    window->resizeCallback = callback;
}

static void framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
    Window* window = glfwGetWindowUserPointer(glfwWindow);
    window->width = width;
    window->height = height;
    glViewport(0, 0, width, height);
    if (window->resizeCallback != NULL) {
        window->resizeCallback(width, height);
    }
}
