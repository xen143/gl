#include "../include/Graphics.h"

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

void shader_setMat4(Shader ID, const char* uniformName, Mat4* mat)
{
    GLuint mvpLoc = glGetUniformLocation(ID, uniformName);
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (const GLfloat*)(mat));
}

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
