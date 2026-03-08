#include <chunk/chunk.h>
#include <algorithm>

void Chunk::GenerateChunk(int groundHeight) {
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
                if (y == 0) {
                    blocks[x][y][z].id = BLOCK_BEDROCK;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else if (y == 64) {
                    blocks[x][y][z].id = BLOCK_DIRT;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else if (y == 63) {
                    blocks[x][y][z].id = BLOCK_DIRT;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else if (y <= groundHeight) {
                    blocks[x][y][z].id = BLOCK_STONE;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else if (y == 65) {
                    blocks[x][y][z].id = BLOCK_GRASS;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else if (y == 0) {
                    blocks[x][y][z].id = BLOCK_BEDROCK;
                    SetBlockTexture(blocks[x][y][z]); // attach XY coords
                } else {
                    blocks[x][y][z].id = BLOCK_AIR;
                }
                
            }
        }
    }
    needsUpdate = true;
}

Block Chunk::GetBlock(int x, int y, int z) const {
    x = std::clamp(x, 0, CHUNK_SIZE_X - 1);
    y = std::clamp(y, 0, CHUNK_SIZE_Y - 1);
    z = std::clamp(z, 0, CHUNK_SIZE_Z - 1);
    return blocks[x][y][z];
}

void Chunk::SetBlock(int x, int y, int z, const Block& block) {
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z) return;

    blocks[x][y][z] = block;
    needsUpdate = true;
}

bool Chunk::IsFaceVisible(int x, int y, int z, BlockFace face) const {
    int nx = x, ny = y, nz = z;

    switch (face) {
        case FACE_FRONT:  nz += 1; break;
        case FACE_BACK:   nz -= 1; break;
        case FACE_LEFT:   nx -= 1; break;
        case FACE_RIGHT:  nx += 1; break;
        case FACE_TOP:    ny += 1; break;
        case FACE_BOTTOM: ny -= 1; break;
        default: break;
    }

    // If neighbor is outside the chunk, assume face is visible
    if (nx < 0 || nx >= CHUNK_SIZE_X ||
        ny < 0 || ny >= CHUNK_SIZE_Y ||
        nz < 0 || nz >= CHUNK_SIZE_Z) return true;

    Block neighbor = GetBlock(nx, ny, nz);
    return neighbor.id == BLOCK_AIR;
}
