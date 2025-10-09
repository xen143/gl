#ifndef CHUNK_H
#define CHUNK_H

#define CHUNK_SIZE 8
#define CHUNK_HEIGHT 8

#include "./Graphics.h"
#include "./Space.h"

typedef enum
{
    BLOCK_AIR = 0,
    BLOCK_SOLID = 1,
} BlockType;

typedef struct
{
    BlockType blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    Vec3 worldPosition;
    VAO VAO;
    VBO VBO;
    EBO EBO;
    int vertexCount;
} Chunk;

typedef struct
{
    Vec3 position;
} ChunkVertex;

Chunk* chunk_create(int chunkX, int chunkZ);
void   chunk_generateMesh(Chunk* chunk);
void   chunk_render(Chunk* chunk);
void   chunk_destroy(Chunk* chunk);

#endif // CHUNK_H
