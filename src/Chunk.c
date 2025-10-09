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
                chunk->blocks[x][y][z] = BLOCK_AIR;

    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int z = 0; z < CHUNK_SIZE; z++)
            chunk->blocks[x][0][z] = BLOCK_SOLID;

    return chunk;
}

void chunk_generateMesh(Chunk* chunk)
{
    ChunkVertex vertices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE * 36];
    int vertexCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (chunk->blocks[x][y][z] != BLOCK_SOLID) continue;

                Vec3 positions[6] =
                {
                    vec3(x, y, z), vec3(x + 1, y, z    ), vec3(x + 1, y, z - 1),
                    vec3(x, y, z), vec3(x + 1, y, z - 1), vec3(x,     y, z - 1),
                };

                for (int i = 0; i < 6; i++)
                {
                    vertices[vertexCount].position = positions[i];
                    vertexCount++;
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

    vao_linkAttrib(chunk->VBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)(0));

    vao_unbind(chunk->VAO);
    vbo_unbind(chunk->VBO);
    ebo_unbind(chunk->EBO);
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
