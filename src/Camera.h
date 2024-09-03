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
	float m_Speed = 15.0f;
	float pitch; float yaw;
	float sensitivity = 0.1f;

public:
	glm::vec3 m_UpVector = { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_CameraPosition;	// Eye
	glm::vec3 m_ViewDirection;	

	bool m_IsMain;

	// Frustum Related
	glm::vec3 m_Right;

	const float m_NEAR = 0.001f;
	const float m_FAR  = 1000.0f;

	glm::mat4 m_ProjectionMatrix;

	int	m_FOV = 60;

	Camera(glm::vec3 cameraPosition, glm::vec3 pointToLookAt, int windowWidth, int windowHeight, bool isMain)
		: m_CameraPosition(cameraPosition),
		m_ViewDirection(glm::normalize(pointToLookAt - m_CameraPosition)),
		m_IsMain(isMain)
	{
		pitch = asin(m_ViewDirection.y);
		yaw = atan2(m_ViewDirection.z, m_ViewDirection.x);

		m_Right = glm::cross(m_UpVector, m_ViewDirection);

		UpdateProjectionMatrix(windowWidth, windowHeight);
	}

	void UpdateProjectionMatrix(int windowWidth, int windowHeight)
	{
		float aspectRatio = (float)windowWidth / windowHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians((float)m_FOV), aspectRatio, m_NEAR, m_FAR);	
	}
	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_CameraPosition, m_CameraPosition + m_ViewDirection, m_UpVector);
	}

	void ProcesssInputs(GLFWwindow* window, double deltaTime)
	{
		//std::cout << "Yaw: " << yaw << " Pitch: " << pitch << std::endl;

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

			// Update yaw and pitch
			yaw += rotX;
			pitch -= rotY; // Note: inverted for typical FPS behavior

			// Clamp the pitch
			if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;

			// Update the view direction
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(-pitch));
			front.y = sin(glm::radians(-pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			m_ViewDirection = glm::normalize(front);

			// Update the right vector
			m_Right = glm::normalize(glm::cross(m_UpVector, m_ViewDirection));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			firstClick = true;
		}
	}
};

#endif
