#ifndef GRASS_CHUNK
#define GRASS_CHUNK

#include <vector>

#include "../Camera/Frustum.h"
#include "Mesh.h"

class GrassChunk
{
private:
	AABB m_ChunkAABB;
	std::vector<glm::vec2> m_GrassPositions;

public:
	AABB getCunkAABB() { return m_ChunkAABB; }

	GrassChunk(glm::vec2 chunkPosition, unsigned int chunkSideLenght, unsigned int GrassObjPerChunk);
	void Render(ShaderProgram grassShaderProgram, Mesh& grassMesh, CameraFrustum frustum);
};
#endif // !GRASS_CHUNK