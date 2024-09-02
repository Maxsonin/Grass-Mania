#ifndef GRASS_RENDERER
#define GRASS_REBDERER

#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Mesh.h"

class GrassRenderer
{
private:
	Mesh m_GrassMesh;

	ShaderProgram m_GrassShaderProgram;

	Camera* m_Camera;

	std::vector<glm::vec3> m_GrassPositions;
	std::vector<int>       m_GrassheightScaleFactor;
public:
	GrassRenderer(Camera* camera);
	void Render();
};

#endif