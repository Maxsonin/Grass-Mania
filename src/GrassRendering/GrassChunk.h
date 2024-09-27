#ifndef GRASS_CHUNK
#define GRASS_CHUNK

#include <vector>

#include "../Camera/Frustum.h"
#include "Mesh.h"

class GrassChunk
{
private:
	glm::vec2 m_Position;
	AABB m_ChunkAABB;
	std::vector<glm::vec2> m_GrassPositions;

public:
	AABB getCunkAABB() { return m_ChunkAABB; }
	glm::vec3 getPosition() const { return glm::vec3(m_Position.x, 0.0f, m_Position.y); }
	GrassChunk(glm::vec2 chunkPosition, int chunkSideLenght, unsigned int GrassObjPerChunk);
	void Render(ShaderProgram grassShaderProgram, Mesh& grassMesh, CameraFrustum frustum);
};
#endif // !GRASS_CHUNK