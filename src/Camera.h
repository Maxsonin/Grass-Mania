#ifndef CAMERA
#define CAMERA

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 m_UpVector = { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_CameraPosition;	// Eye
	glm::vec3 m_DirectionToLook;				 
public:
	Camera(glm::vec3 cameraPosition, glm::vec3 pointToLookAt)
		: m_CameraPosition(cameraPosition), m_DirectionToLook(pointToLookAt - m_CameraPosition) {}

	glm::mat4 getProjectionMatrix(int windowWidth, int windowHeight)
	{
		float aspectRatio = (float)windowWidth / windowHeight;
		return glm::perspective(glm::radians((float)m_FOV), aspectRatio, m_NEAR, m_FAR);
	}
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_CameraPosition, m_CameraPosition + m_DirectionToLook, m_UpVector);
	}

	int			m_FOV  = 60;
	const float m_NEAR = 0.01f;
	const float m_FAR  = 1000.0f;
};

#endif
