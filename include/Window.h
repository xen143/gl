#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./Space.h"

typedef struct
{
    GLFWwindow* glfwWindow;
    int width;
    int height;
} Window;

Window window_create(int width, int height, const char* title);
void   window_swapBuffers(Window* window);
void   window_destroy(Window* window);
bool   window_shouldClose(Window* window);
bool   window_isKeyPressed(Window* window, int key);

void window_showCursor(Window* window);
void window_hideCursor(Window* window);
void window_centerCursor(Window* window);

Vec3 window_getMovementVec(Window* window);

#endif // WINDOW_H
