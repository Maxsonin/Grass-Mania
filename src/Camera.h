#ifndef CAMERA
#define CAMERA

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 m_UpVector = { 0.0f, 1.0f, 0.0f }; // Up direction is positive Y axis
	glm::vec3 m_Eye;							 // Camera position
	glm::vec3 m_DirectionToLook;				 // Camera is looking in this direction
public:
	Camera(glm::vec3 eye, glm::vec3 directionToLook);

	glm::mat4 getProjectionMatrix(int windowWidth, int windowHeight)
	{
		return glm::perspective(m_FOV, (float)windowWidth/windowHeight, m_NEAR, m_FAR)
	}
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_Eye, m_Eye + m_DirectionToLook, m_UpVector);
	}

	unsigned int m_FOV = 60;
	const float m_NEAR = 0.01f;
	const float m_FAR = 1000.0f;
};

#endif
