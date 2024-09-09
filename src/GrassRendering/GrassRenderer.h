#ifndef GRASS_RENDERER
#define GRASS_REBDERER

#include <vector>
#include <glm/glm.hpp>

#include "../Camera.h"
#include "Mesh.h"
#include "../Utility/Frustum.h"
#include "GrassChunk.h"

class GrassRenderer
{
private:
	Mesh m_GrassMesh;

	ShaderProgram m_GrassShaderProgram;
	ShaderProgram m_WorldShaderProgram;
	ShaderProgram m_CameraShaderProgram;

	CameraManager* m_CameraManager;

	const unsigned int m_MeshesPerChunk = 1500;
	const unsigned int m_ChunkSideLenght = 25;

	CameraFrustum frustum;

	std::vector<GrassChunk> m_GrassChunks;

	unsigned int m_NumOfChunks = 36;


	GLuint worldVAO;

public:
	GrassRenderer(CameraManager* cameraManager);
	void Render();
};

#endif