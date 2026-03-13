#ifndef RAYCAST_H
#define RAYCAST_H

bool BlockRaycast(const Chunk& chunk, glm::vec3 Borigin, glm::vec3 dir, float maxDistance, glm::ivec3& hitBlock, BlockFace& hitFace);

glm::ivec3 FaceToDirection(BlockFace face);

#endif