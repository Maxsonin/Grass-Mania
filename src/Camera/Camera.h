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
#include <vector>

class Camera
{
private:
	// Move&Rotate Settings
	float m_MovementSpeed    = 15.0f;
	float m_MouseSensitivity = 0.1f; 
	float m_KeySensetivity   = 25.0f;

	glm::mat4 m_ProjectionMatrix;

public:
	std::string m_CameraName;

	float pitch; float yaw; float roll;

	const glm::vec3 m_UpVector = { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_CameraPosition;	// Eye
	glm::vec3 m_ViewDirection;	
	glm::vec3 m_RightVector;

	// Camera managment variables
	bool m_IsMain;
	bool m_InUse = false;

	const float m_NEAR = 0.001f;
	const float m_FAR  = 1'000.0f;

	int	m_FOV = 60;

	// Frustum Related
	const float m_FRUSTUM_NEAR = 1.0f;
	const float m_FRUSTUM_FAR  = 400.0f;

	const float m_DistOfLowPolly = 350.0f;

	// Widnow Settings
	int m_windowWidth, m_windowHeight;
	float aspectRatio = 1.0f;

	Camera(std::string cameraName, glm::vec3 cameraPosition, glm::vec3 pointToLookAt, int windowWidth, int windowHeight, bool isMain)
		: m_CameraName(cameraName),
		m_CameraPosition(cameraPosition),
		m_windowWidth(windowWidth), m_windowHeight(windowHeight),
		m_IsMain(isMain)
	{
		m_ViewDirection = glm::normalize(pointToLookAt - m_CameraPosition);

		pitch = asin(m_ViewDirection.y);
		yaw   = atan2(m_ViewDirection.z, m_ViewDirection.x);

		m_RightVector = glm::cross(m_UpVector, m_ViewDirection);

		UpdateProjectionMatrix(windowWidth, windowHeight);
	}

	// Matrices
	void UpdateProjectionMatrix(int windowWidth, int windowHeight)
	{
		m_windowWidth = windowWidth; m_windowHeight = windowHeight;
		aspectRatio = (float)windowWidth / windowHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians((float)m_FOV), aspectRatio, m_NEAR, m_FAR);	
	}
	inline glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_CameraPosition, m_CameraPosition + m_ViewDirection, m_UpVector);
	}
	inline glm::mat4 GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	void ProcesssInputs(GLFWwindow* window, double deltaTime)
	{
	#pragma region Key Binding
		float travelDistance = m_MovementSpeed * (float)deltaTime;

		// Basic movements
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
			m_MovementSpeed = 25.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			m_MovementSpeed = 15.0f;
		}

		// Camera Angle movement
		static bool isAngleChanged = false;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			yaw += m_KeySensetivity * deltaTime;
			isAngleChanged = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)
		{
			yaw -= m_KeySensetivity * deltaTime;
			isAngleChanged = true;
		}
		// Multiplied by 0.75 for better result
		if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS)
		{
			pitch -= m_KeySensetivity * 0.75f * deltaTime; 
			isAngleChanged = true;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS)
		{
			pitch += m_KeySensetivity * 0.75f * deltaTime;
			isAngleChanged = true;
		}
		if (isAngleChanged = true)
		{
			UpdateCameraRotation();
			isAngleChanged = false; // reset
		}
#pragma endregion

	#pragma region Mouse Inputs
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
			float offsetX = mouseX - origMouseX;
			float offsetY = origMouseY - mouseY; // Reversed since y-coordinates range from bottom to top

			// Update for the next frame
			origMouseX = mouseX;
			origMouseY = mouseY;

			// Apply sensitivity scaling
			float rotX = offsetX * m_MouseSensitivity;
			float rotY = offsetY * m_MouseSensitivity;

			// Update
			yaw   += rotX;
			pitch -= rotY;

			UpdateCameraRotation();
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			firstClick = true;
		}
#pragma endregion
	}

private:
	void UpdateCameraRotation()
	{
		// Clamp the pitch
		if (pitch > 89.0f)  pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		// Update the view direction
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(-pitch));
		direction.y = sin(glm::radians(-pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_ViewDirection = glm::normalize(direction);

		// Update the right vector
		m_RightVector = glm::normalize(glm::cross(m_UpVector, m_ViewDirection));
	}
};

enum class CameraType
{
	BASIC_CAMERA, // No need for special handling
	MAIN_CAMERA
};

class CameraManager
{
private:
	Camera* m_MainCamera  = nullptr;
	Camera* m_CameraInUse = nullptr;
	std::vector<Camera*> m_Cameras;
	int m_CurrentCameraIndex = 0; // Track the index of the currently active camera

public:
	void AddCamera(Camera* newCamera, CameraType cameraType, bool setInUse = false)
	{
		if (newCamera != nullptr)
		{
			m_Cameras.push_back(newCamera);

			switch (cameraType)
			{
			case CameraType::BASIC_CAMERA:
				break;

			case CameraType::MAIN_CAMERA:
				SetMainCamera(newCamera->m_CameraName);
				break;
			}

			if (setInUse == true)
			{
				newCamera->m_InUse = true;
				m_CameraInUse = newCamera;
				m_CurrentCameraIndex = m_Cameras.size() - 1;
			}
		}
		else
		{
			std::cout << "The new Camera is not initialized! CHANGES NOT APPLIED\n";
		}
	}

	Camera* GetMainCamera() const { return m_MainCamera; }
	Camera* GetCameraInUse() const { return m_CameraInUse; }
	Camera* GetCamera(const std::string& cameraName) const
	{
		for (const auto& camera : m_Cameras)
		{
			if (camera->m_CameraName == cameraName)
				return camera;
		}

		std::cout << "Camera with name: " << cameraName << " is NOT found\n";
		return nullptr;
	}

	void SetMainCamera(const std::string& cameraName)
	{
		Camera* cameraToChange = GetCamera(cameraName);

		if (cameraToChange != nullptr)
		{
			if (m_MainCamera != nullptr)
			{
				m_MainCamera->m_IsMain = false;
			}

			m_MainCamera = cameraToChange;
			cameraToChange->m_IsMain = true;
		}
		else
		{
			std::cout << "The new MAIN Camera is not Initialized! CHANGES NOT APPLIED\n";
		}
	}
	void SetCameraInUse(const std::string& cameraName)
	{
		Camera* cameraToChange = GetCamera(cameraName);

		if (cameraToChange != nullptr)
		{
			if (m_CameraInUse != nullptr)
			{
				m_CameraInUse->m_InUse = false;
			}

			m_CameraInUse = cameraToChange;
			cameraToChange->m_InUse = true;
		}
		else
		{
			std::cout << "The new Camera In Use is not Initialized! CHANGES NOT APPLIED\n";
		}
	}

	void RemoveCamera(const std::string& cameraNameToRemove)
	{
		auto it = std::remove_if(m_Cameras.begin(), m_Cameras.end(),
			[cameraNameToRemove](Camera* camera)
		{
			return camera->m_CameraName == cameraNameToRemove;
		});

		if (it != m_Cameras.end())
		{
			if (m_MainCamera && m_MainCamera->m_CameraName == cameraNameToRemove)
			{
				std::cout << "Removing main camera.\n";
				m_MainCamera = nullptr;
			}
			if (m_CameraInUse && m_CameraInUse->m_CameraName == cameraNameToRemove)
			{
				std::cout << "Removing camera in use.\n";
				m_CameraInUse = nullptr;
			}

			m_Cameras.erase(it, m_Cameras.end());
		}
		else
		{
			std::cout << "Camera not found.\n";
		}
	}

	void ProccesInputs(GLFWwindow* window, float deltaTime)
	{
		static int currentCameraIndex = 0;

		static bool cameraSwitching = false;
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			if (!cameraSwitching)
			{
				unsigned int indexOfNextCamera = m_CurrentCameraIndex + 1;
				if (indexOfNextCamera >= m_Cameras.size())
				{
					indexOfNextCamera = 0;
				}
				m_CameraInUse->m_InUse = false;
				m_CameraInUse = m_Cameras[indexOfNextCamera];
				m_CameraInUse->m_InUse = true;

				m_CurrentCameraIndex = indexOfNextCamera;
				cameraSwitching = true;
			}
		}
		else
		{
			cameraSwitching = false;
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			m_MainCamera->ProcesssInputs(window, deltaTime);
		}
		else
		{
			m_CameraInUse->ProcesssInputs(window, deltaTime);
		}
	}
};
#endif