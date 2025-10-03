#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

typedef GLuint Shader;
typedef GLuint VAO;
typedef GLuint VBO;
typedef GLuint EBO;

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
    "void main()\n"
    "{\n"
    "  vertCol = aCol;\n"
    "  gl_Position = vec4(aPos, 1.f);\n"
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
    -0.5f,  -0.5f, 0.f, 1.f, 0.f, 0.f,
     0.0f,  -0.5f, 0.f, 0.f, 1.f, 0.f,
     0.5f,  -0.5f, 0.f, 0.f, 0.f, 1.f,
    -0.25f,  0.f,  0.f, 1.f, 1.f, 0.f,
     0.25f,  0.f,  0.f, 1.f, 0.f, 1.f,
     0.0f,   0.5f, 0.f, 0.f, 1.f, 1.f,
};
const GLuint indices[] =
{
    0, 1, 3,
    1, 2, 4,
    3, 4, 5,
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

    Shader shader = shader_create(vertexShaderSource, fragmentShaderSource);

    VAO VAO = vao_create();
    VBO VBO = vbo_create(vertices, sizeof(vertices));
    EBO EBO = ebo_create(indices, sizeof(indices));

    vao_bind(VAO);
    vbo_bind(VBO);
    ebo_bind(EBO);

    vao_linkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(0));
    vao_linkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLuint)));

    vao_unbind(VAO);
    vbo_unbind(VBO);
    ebo_unbind(EBO);

    glClearColor(0.f, 0.5f, 0.5f, 1.f);
    while (!glfwWindowShouldClose(window))
    {
        shader_use(shader);
        glClear(GL_COLOR_BUFFER_BIT);
        vao_bind(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        vao_unbind(VAO);
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
