#include "GrassChunk.h"

#include <glm/glm.hpp>
#include "../Utility/Random.h"
#include <glm/gtc/matrix_transform.hpp>

GrassChunk::GrassChunk(glm::vec2 chunkPosition, unsigned int chunkSideLenght, unsigned int GrassObjPerChunk)
	: m_ChunkAABB(glm::vec3(chunkPosition.x, 0.0f, chunkPosition.y),
				  glm::vec3(chunkPosition.x + 0.1f, 5.0f, chunkPosition.y + 0.f))
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
    for (size_t i = 0; i < m_GrassPositions.size(); i++)
    {
        glm::vec2 grassPosition = m_GrassPositions[i];

        // Create AABB for the grass instance
        glm::vec3 minPoint = glm::vec3(grassPosition.x, 0.0f, grassPosition.y);
        glm::vec3 maxPoint = glm::vec3(grassPosition.x + 0.1f, 5.0f, grassPosition.y + 0.3f);
        AABB grassAABB(minPoint, maxPoint);

        // Perform frustum culling
        if (!grassAABB.isOnFrustum(frustum)) continue; // Skip rendering this grass mesh if it's outside the frustum

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(grassPosition.x, 0.0f, grassPosition.y));
        grassShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

        glCheckError();
        grassMesh.Render(grassShaderProgram);
        glCheckError();
    }
}