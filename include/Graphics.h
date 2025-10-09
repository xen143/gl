#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h> 

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

#endif // GRAPHICS_H
