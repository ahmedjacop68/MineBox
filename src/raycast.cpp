#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chunk/chunk.h>
#include <block/block.h>
#include <raycast/raycast.hpp>

//3D DDA math for raycasting
bool BlockRaycast(const Chunk& chunk, glm::vec3 Borigin, glm::vec3 dir, float maxDistance, glm::ivec3& hitBlock, BlockFace& hitFace)
    {
        glm::vec3 origin = Borigin + 0.01f;

        glm::ivec3 block = glm::ivec3(
            floor(origin.x),
            floor(origin.y),
            floor(origin.z)
        );

        glm::vec3 deltaDist = glm::vec3(
            dir.x == 0 ? 1e30f : fabs(1.0f / dir.x),
            dir.y == 0 ? 1e30f : fabs(1.0f / dir.y),
            dir.z == 0 ? 1e30f : fabs(1.0f / dir.z)
        );

        glm::ivec3 step;
        glm::vec3 sideDist;

        for (int i = 0; i < 3; i++)
        {
            if (dir[i] < 0)
            {
                step[i] = -1;
                sideDist[i] = (origin[i] - block[i]) * deltaDist[i];
            }
            else
            {
                step[i] = 1;
                sideDist[i] = (block[i] + 1.0f - origin[i]) * deltaDist[i];
            }
        }

        float dist = 0.0f;

        for (int i = 0; i < 128; i++)
        {
            int axis;

            if (sideDist.x < sideDist.y && sideDist.x < sideDist.z)
                axis = 0;
            else if (sideDist.y < sideDist.z)
                axis = 1;
            else
                axis = 2;

            block[axis] += step[axis];
            dist = sideDist[axis];
            sideDist[axis] += deltaDist[axis];

            if (block.x < 0 || block.x >= CHUNK_SIZE_X ||
                block.y < 0 || block.y >= CHUNK_SIZE_Y ||
                block.z < 0 || block.z >= CHUNK_SIZE_Z)
                continue;

            Block b = chunk.GetBlock(block.x, block.y, block.z);

            if (b.id != BLOCK_AIR)
            {
                hitBlock = block;

                if (axis == 0)
                    hitFace = (step.x > 0) ? FACE_LEFT : FACE_RIGHT;
                else if (axis == 1)
                    hitFace = (step.y > 0) ? FACE_BOTTOM : FACE_TOP;
                else
                    hitFace = (step.z > 0) ? FACE_BACK : FACE_FRONT;

                return true;
            }
        }

        return false;
    }

//offset for placing blocks
glm::ivec3 FaceToDirection(BlockFace face)
    {
        switch (face)
        {
            case FACE_FRONT:  return glm::ivec3(0, 0, 1);
            case FACE_BACK:   return glm::ivec3(0, 0, -1);
            case FACE_LEFT:   return glm::ivec3(-1, 0, 0);
            case FACE_RIGHT:  return glm::ivec3(1, 0, 0);
            case FACE_TOP:    return glm::ivec3(0, 1, 0);
            case FACE_BOTTOM: return glm::ivec3(0, -1, 0);
        }
        return glm::ivec3(0);
    }