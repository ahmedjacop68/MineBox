#pragma once
#include <glm/glm.hpp>

enum BlockID {
    BLOCK_AIR = 0,
    BLOCK_STONE = 1,
    BLOCK_GRASS = 2,
    BLOCK_DIRT = 3,
    BLOCK_COBBLESTONE = 4,
    BLOCK_PLANKS = 5,
    BLOCK_OAK_SAPLING = 6,
    BLOCK_BEDROCK = 7,
    BLOCK_SAND = 12,
    BLOCK_GRAVEL = 13,
    BLOCK_GOLD_ORE = 14,
    BLOCK_IRON_ORE = 15,
    BLOCK_COAL_ORE = 16,
};

//Named faces (stable order, do not change)
enum BlockFace {
    FACE_FRONT = 0,
    FACE_BACK,
    FACE_LEFT,
    FACE_RIGHT,
    FACE_TOP,
    FACE_BOTTOM,
    FACE_COUNT
};

struct Block {
    BlockID id = BLOCK_AIR;

    glm::ivec2 texXY = glm::ivec2(0, 0);            //Default texture
    glm::ivec2 faceTex[FACE_COUNT] = {};            //Optional per-face textures

    glm::vec4 faceTint[FACE_COUNT] = {
        glm::vec4(1.0f),
        glm::vec4(1.0f),
        glm::vec4(1.0f),
        glm::vec4(1.0f),
        glm::vec4(1.0f),
        glm::vec4(1.0f)
    };

    int tileSize = 16;

    glm::vec4 Tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::ivec2 GetFaceTexXY(BlockFace face) const {
        const glm::ivec2 &t = faceTex[face];
        return (t.x != 0 || t.y != 0) ? t : texXY;
    }

    glm::vec2 GetUVMin(const glm::ivec2 &tex, int atlasSize = 256) const {
        float u = (float)tex.x / atlasSize;
        float v = 1.0f - ((float)(tex.y + tileSize) / atlasSize);
        return glm::vec2(u, v);
    }

    glm::vec2 GetUVMax(const glm::ivec2 &tex, int atlasSize = 256) const {
        float u = (float)(tex.x + tileSize) / atlasSize;
        float v = 1.0f - ((float)tex.y / atlasSize);
        return glm::vec2(u, v);
    }
};


inline void SetBlockTexture(Block &block) {

    for (int i = 0; i < FACE_COUNT; i++)
        block.faceTex[i] = glm::ivec2(0,0);


    switch (block.id) {
        case BLOCK_STONE:
            block.texXY = glm::ivec2(16, 240);
            break;

        case BLOCK_GRASS:
            block.texXY = glm::ivec2(48, 240);                 // sides
            block.faceTex[FACE_TOP] = glm::ivec2(128, 208);     // grass top
            block.faceTint[FACE_TOP] = glm::vec4(0.569, 0.741, 0.349, 1); //Grass top face Tint
            block.faceTex[FACE_BOTTOM] = glm::ivec2(32, 240);  // dirt
            break;

        case BLOCK_DIRT:
            block.texXY = glm::ivec2(32, 240);
            break;

        case BLOCK_COBBLESTONE:
            block.texXY = glm::ivec2(0, 224);
            break;

        case BLOCK_PLANKS:
            block.texXY = glm::ivec2(64, 240);
            break;

        case BLOCK_OAK_SAPLING:
            block.texXY = glm::ivec2(240, 240);
            break;

        case BLOCK_BEDROCK:
            block.texXY = glm::ivec2(16, 224);
            break;

        case BLOCK_SAND:
            block.texXY = glm::ivec2(32, 224);
            break;

        case BLOCK_GRAVEL:
            block.texXY = glm::ivec2(48, 224);
            break;

        case BLOCK_GOLD_ORE:
            block.texXY = glm::ivec2(0, 208);
            break;

        case BLOCK_IRON_ORE:
            block.texXY = glm::ivec2(16, 208);
            break;

        case BLOCK_COAL_ORE:
            block.texXY = glm::ivec2(32, 208);
            break;

        default:
            break;
    }
}
