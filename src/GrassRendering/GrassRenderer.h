#ifndef GRASS_RENDERER
#define GRASS_REBDERER

#include <vector>
#include <glm/glm.hpp>

#include "../Camera.h"
#include "Mesh.h"
#include "../Utility/Frustum.h"

class GrassRenderer
{
private:
	Mesh m_GrassMesh;

	ShaderProgram m_GrassShaderProgram;

	Camera* m_Camera;

	int grassCount;

	CameraFrustum frustum;

	std::vector<glm::vec3> m_GrassPositions;
	std::vector<int>       m_GrassheightScaleFactor;
public:
	GrassRenderer(Camera* camera);
	void Render(Camera* debugCam);
};

#endif