#ifndef GRASS_RENDERER
#define GRASS_RENDERER

#include <vector>

#include <glm/glm.hpp>

#include "../Camera/Camera.h"
#include "../Camera/Frustum.h"
#include "GrassChunk.h"
#include "Mesh.h"

class GrassRenderer
{
private:
	Mesh m_GrassMesh;
	Mesh m_LowPollyGrassMesh;

	ShaderProgram m_GrassShaderProgram;
	ShaderProgram m_CameraShaderProgram;

	CameraManager* m_CameraManager;

	const int m_MeshesPerChunk = 700;
	const int m_ChunkSideLenght = 25;

	CameraFrustum frustum;

	std::vector<GrassChunk> m_GrassChunks;

	unsigned int m_NumOfLayars = 17;

public:
	GrassRenderer(CameraManager* cameraManager);
	void Render();
};

#endif