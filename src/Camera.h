#ifndef CAMERA
#define CAMERA

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Camera
{
private:
	glm::vec3 m_UpVector = { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_CameraPosition;	// Eye
	glm::vec3 m_ViewDirection;		

	float m_Speed	   = 15.0f;
	const float m_NEAR = 0.01f;
	const float m_FAR  = 1000.0f;

	float sensitivity = 0.1f;

	glm::mat4 m_ProjectionMatrix;

public:
	int	m_FOV = 60;

	Camera(glm::vec3 cameraPosition, glm::vec3 pointToLookAt, int windowWidth, int windowHeight)
		: m_CameraPosition(cameraPosition), m_ViewDirection(pointToLookAt - m_CameraPosition) 
	{ UpdateProjectionMatrix(windowWidth, windowHeight); }

	void UpdateProjectionMatrix(int windowWidth, int windowHeight)
	{
		float aspectRatio = (float)windowWidth / windowHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians((float)m_FOV), aspectRatio, m_NEAR, m_FAR);	
	}

	glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_CameraPosition, m_CameraPosition + m_ViewDirection, m_UpVector);
	}

	void ProcesssInputs(GLFWwindow* window, double deltaTime)
	{
		float travelDistance = m_Speed * (float)deltaTime;

		// Basic Movements
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * glm::normalize(m_ViewDirection);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * -glm::normalize(glm::cross(m_ViewDirection, m_UpVector));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * glm::normalize(-m_ViewDirection);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * glm::normalize(glm::cross(m_ViewDirection, m_UpVector));
		}

		// Height Controll
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * m_UpVector;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			m_CameraPosition += travelDistance * -m_UpVector;
		}

		// Speed Controll
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			m_Speed = 25.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			m_Speed = 15.0f;
		}

		// Handles mouse inputs
		static bool firstClick = true;
		static double origMouseX, origMouseY;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (firstClick)
			{
				origMouseX = mouseX; origMouseY = mouseY;
				firstClick = false;
			}

			// Calculate the offset since the last frame
			double offsetX = mouseX - origMouseX;
			double offsetY = origMouseY - mouseY; // Reversed since y-coordinates range from bottom to top

			// Update for the next frame
			origMouseX = mouseX;
			origMouseY = mouseY;

			// Apply sensitivity scaling
			float rotX = static_cast<float>(offsetX) * sensitivity;
			float rotY = static_cast<float>(offsetY) * sensitivity;

			// Create a quaternion representing the new orientation
			glm::vec3 right = glm::normalize(glm::cross(m_ViewDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::quat pitch = glm::angleAxis(glm::radians(rotY), right);
			glm::quat yaw = glm::angleAxis(glm::radians(-rotX), glm::vec3(0.0f, 1.0f, 0.0f));

			// Update the view direction
			m_ViewDirection = glm::normalize(glm::rotate(yaw * pitch, m_ViewDirection));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			firstClick = true;
		}
	}
};

#endif
