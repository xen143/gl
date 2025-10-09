#include "../include/Chunk.h"

Chunk* chunk_create(int chunkX, int chunkZ)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    if (chunk == NULL) return NULL;

    chunk->worldPosition = vec3(chunkX * CHUNK_SIZE, 0, chunkZ * CHUNK_SIZE);
    chunk->vertexCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                bool isSolid = x % 8 > 3 && y % 8 > 3 && z % 8 > 3;
                chunk->blocks[x][y][z] = isSolid ? BLOCK_SOLID : BLOCK_AIR;
            }

    return chunk;
}

void chunk_generateMesh(Chunk* chunk)
{
    size_t maxVertices = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE * 36;
    ChunkVertex* vertices = malloc(maxVertices * sizeof(ChunkVertex));
    if (vertices == NULL) return;

    int vertexCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (chunk->blocks[x][y][z] != BLOCK_SOLID) continue;

                bool renderFront  = (z == CHUNK_SIZE - 1) || (chunk->blocks[x][y][z + 1] == BLOCK_AIR);
                bool renderBack   = (z == 0) || (chunk->blocks[x][y][z - 1] == BLOCK_AIR);
                bool renderRight  = (x == CHUNK_SIZE - 1) || (chunk->blocks[x + 1][y][z] == BLOCK_AIR);
                bool renderLeft   = (x == 0) || (chunk->blocks[x - 1][y][z] == BLOCK_AIR);
                bool renderTop    = (y == CHUNK_HEIGHT - 1) || (chunk->blocks[x][y + 1][z] == BLOCK_AIR);
                bool renderBottom = (y == 0) || (chunk->blocks[x][y - 1][z] == BLOCK_AIR);

                Vec3 positions[36] =
                {
                    vec3(x,     y,     z + 1), vec3(x + 1, y,     z + 1), vec3(x + 1, y + 1, z + 1), // Front
                    vec3(x,     y,     z + 1), vec3(x + 1, y + 1, z + 1), vec3(x,     y + 1, z + 1), // Front
                    vec3(x + 1, y,     z + 1), vec3(x + 1, y,     z    ), vec3(x + 1, y + 1, z    ), // Right
                    vec3(x + 1, y,     z + 1), vec3(x + 1, y + 1, z    ), vec3(x + 1, y + 1, z + 1), // Right
                    vec3(x + 1, y,     z    ), vec3(x,     y,     z    ), vec3(x,     y + 1, z    ), // Back
                    vec3(x + 1, y,     z    ), vec3(x,     y + 1, z    ), vec3(x + 1, y + 1, z    ), // Back
                    vec3(x,     y,     z    ), vec3(x,     y,     z + 1), vec3(x,     y + 1, z + 1), // Left
                    vec3(x,     y,     z    ), vec3(x,     y + 1, z + 1), vec3(x,     y + 1, z    ), // Left
                    vec3(x,     y + 1, z + 1), vec3(x + 1, y + 1, z + 1), vec3(x + 1, y + 1, z    ), // Top
                    vec3(x,     y + 1, z + 1), vec3(x + 1, y + 1, z    ), vec3(x,     y + 1, z    ), // Top
                    vec3(x + 1, y,     z    ), vec3(x,     y,     z    ), vec3(x,     y,     z + 1), // Bottom
                    vec3(x + 1, y,     z    ), vec3(x,     y,     z + 1), vec3(x + 1, y,     z + 1), // Bottom
                };
                Vec3 normals[36] =
                {
                    vec3( 0,  0,  1), vec3( 0,  0,  1), vec3( 0,  0,  1), // Front
                    vec3( 0,  0,  1), vec3( 0,  0,  1), vec3( 0,  0,  1), // Front
                    vec3( 1,  0,  0), vec3( 1,  0,  0), vec3( 1,  0,  0), // Right
                    vec3( 1,  0,  0), vec3( 1,  0,  0), vec3( 1,  0,  0), // Right
                    vec3( 0,  0, -1), vec3( 0,  0, -1), vec3( 0,  0, -1), // Back
                    vec3( 0,  0, -1), vec3( 0,  0, -1), vec3( 0,  0, -1), // Back
                    vec3(-1, 0,   0), vec3(-1,  0,  0), vec3(-1,  0,  0), // Left
                    vec3(-1, 0,   0), vec3(-1,  0,  0), vec3(-1,  0,  0), // Left
                    vec3( 0,  1,  0), vec3( 0,  1,  0), vec3( 0,  1,  0), // Top
                    vec3( 0,  1,  0), vec3( 0,  1,  0), vec3( 0,  1,  0), // Top
                    vec3( 0, -1,  0), vec3( 0, -1,  0), vec3( 0, -1,  0), // Bottom
                    vec3( 0, -1,  0), vec3( 0, -1,  0), vec3( 0, -1,  0), // Bottom
                };

                for (int face = 0; face < 6; face++)
                {
                    bool shouldRender = false;
                    switch(face) {
                        case 0: shouldRender = renderFront; break;
                        case 1: shouldRender = renderRight; break;  
                        case 2: shouldRender = renderBack; break;
                        case 3: shouldRender = renderLeft; break;
                        case 4: shouldRender = renderTop; break;
                        case 5: shouldRender = renderBottom; break;
                    }
                    
                    if (shouldRender) {
                        for (int i = 0; i < 6; i++) {
                            int index = face * 6 + i;
                            vertices[vertexCount].position = positions[index];
                            vertices[vertexCount].normal = normals[index];
                            vertexCount++;
                        }
                    }
                }
            }
        }
    }

    chunk->vertexCount = vertexCount;

    GLuint indices[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        indices[i] = i;
    }

    chunk->VAO = vao_create();
    chunk->VBO = vbo_create((GLfloat*)vertices, vertexCount * sizeof(ChunkVertex));
    chunk->EBO = ebo_create(indices, vertexCount * sizeof(GLuint));

    vao_bind(chunk->VAO);
    vbo_bind(chunk->VBO);
    ebo_bind(chunk->EBO);

    vao_linkAttrib(chunk->VBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(0));
    vao_linkAttrib(chunk->VBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    vao_unbind(chunk->VAO);
    vbo_unbind(chunk->VBO);
    ebo_unbind(chunk->EBO);

    free(vertices);
}

void chunk_render(Chunk* chunk)
{
    vao_bind(chunk->VAO);
    glDrawElements(GL_TRIANGLES, chunk->vertexCount, GL_UNSIGNED_INT, 0);
    vao_unbind(chunk->VAO);
}

void chunk_destroy(Chunk* chunk)
{
    if (chunk == NULL) return;
    vao_delete(&chunk->VAO);
    vbo_delete(&chunk->VBO);
    ebo_delete(&chunk->EBO);
    free(chunk);
}
