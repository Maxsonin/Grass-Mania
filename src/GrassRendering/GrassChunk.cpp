#include "GrassChunk.h"

#include <glm/glm.hpp>
#include "../Utility/Random.h"
#include <glm/gtc/matrix_transform.hpp>

GrassChunk::GrassChunk(glm::vec2 chunkPosition, unsigned int chunkSideLenght, unsigned int GrassObjPerChunk)
	: m_ChunkAABB(glm::vec3(chunkPosition.x, 0.0f, chunkPosition.y),
				  glm::vec3(chunkPosition.x + chunkSideLenght, 5.0f, chunkPosition.y + chunkSideLenght)),
    m_Position(chunkPosition)
{
	m_GrassPositions.reserve(GrassObjPerChunk);

	unsigned int startPosX = chunkPosition.x; unsigned int startPosZ = chunkPosition.y;
	unsigned int endPosX = chunkPosition.x + chunkSideLenght;
	unsigned int endPosZ = chunkPosition.y + chunkSideLenght;
	for (size_t i = 0; i < GrassObjPerChunk; i++)
	{
		float x = Random::GenerateFloat(startPosX, endPosX);
		float z = Random::GenerateFloat(startPosZ, endPosZ);

		m_GrassPositions.push_back(glm::vec2(x, z));
	}
}

void GrassChunk::Render(ShaderProgram grassShaderProgram, Mesh& grassMesh, CameraFrustum frustum)
{
    std::vector<glm::vec2> visibleGrassPositions;
    visibleGrassPositions.reserve(m_GrassPositions.size());

    AABB grassAABB(glm::vec3(0.0f), glm::vec3(0.1f, 5.0f, 0.3f)); // info from grass.obj

    // Perform frustum culling for each grass blade
    for (const auto& grassPosition : m_GrassPositions)
    {
        glm::vec3 minPoint = glm::vec3(grassPosition.x, 0.0f, grassPosition.y);
        glm::vec3 maxPoint = glm::vec3(grassPosition.x + 0.1f, 5.0f, grassPosition.y + 0.3f);
        grassAABB.Update(minPoint, maxPoint);

        if (grassAABB.isOnFrustum(frustum))
        {
            visibleGrassPositions.push_back(grassPosition);
        }
    }

    if (!visibleGrassPositions.empty())
    {
        grassMesh.SetInstanceData(visibleGrassPositions);
        glCheckError();

        grassMesh.RenderInstanced(grassShaderProgram, visibleGrassPositions.size());
        glCheckError();
    }
}