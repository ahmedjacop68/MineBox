#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <block/block.h>

const int CHUNK_SIZE_X = 16;
const int CHUNK_SIZE_Y = 128;
const int CHUNK_SIZE_Z = 16;

struct Chunk {
    Block blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z]; // 3D array of blocks
    glm::ivec3 position; // chunk position in world coordinates
    bool needsUpdate = true; // if mesh needs regeneration

    //Generate a chunk
    void GenerateChunk(int groundHeight = 64);

    //Get a block at local coords
    Block GetBlock(int x, int y, int z) const;

    //Set a block at local coords
    void SetBlock(int x, int y, int z, const Block& block);

    //
    bool IsFaceVisible(int x, int y, int z, BlockFace face) const;

};

