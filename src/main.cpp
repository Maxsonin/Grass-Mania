#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "Utility/Debugging.h"

#include "GrassRendering/Mesh.h"
#include "Camera.h"
#include "GrassRendering/GrassRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    // Retrieve the Camera pointer from the window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) { camera->UpdateProjectionMatrix(width, height); }
}

int main()
{
#pragma region Initialization

    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed to initialize GLFW";
        exit(EXIT_FAILURE);
    }

    // Set GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);

    const unsigned int WINDOW_WIDTH  = 1000;
    const unsigned int WINDOW_HEIGHT = 1000;
    GLFWwindow* applicationWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Grass Mania", NULL, NULL);
    if (!applicationWindow)
    {
        std::cerr << "ERROR: Failed to create GLFW window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(applicationWindow);

    if (gl3wInit())
    {
        std::cerr << "ERROR: Failed to Initialize gl3w";
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(applicationWindow, framebuffer_size_callback);

    // Settings
    glEnable(GL_DEPTH_TEST);

    //glfwSwapInterval(0); // Disable V-Sync

#pragma endregion

    glm::vec3 mainCamPosition = { 0.0f, 20.0f, 0.0f }; glm::vec3 mainCamTarget   = { 50.0f, 0.0f, 50.0f };
    Camera mainCamera(mainCamPosition, mainCamTarget, WINDOW_WIDTH, WINDOW_HEIGHT, true);

    glm::vec3 debugCamPosition = { 80.0f, 150.0f, 80.0f }; glm::vec3 debugCamTarget = { 70.0f, 0.0f, 70.0f };
    Camera debugCamera(debugCamPosition, debugCamTarget, WINDOW_WIDTH, WINDOW_HEIGHT, false);

    // Pass the Camera pointer to the GLFW window
    glfwSetWindowUserPointer(applicationWindow, &mainCamera);

    GrassRenderer grassRenderer(&mainCamera);

    static int frameCount = 0, FPS = 0;
    static double previousTime = 0.0, deltaTime = 1.0;        // For FPS count
    static double previousTimeFrame = 0.0, elapsedTime = 0.0; // For between frame time
    while (!glfwWindowShouldClose(applicationWindow))
    {
        // Update deltaTime
        deltaTime = glfwGetTime() - previousTimeFrame;
        previousTimeFrame = glfwGetTime();

        // PRE DRAW
        glfwPollEvents();
        if (mainCamera.m_IsMain || glfwGetKey(applicationWindow, GLFW_KEY_T) != GLFW_PRESS)
        {
            mainCamera.ProcesssInputs(applicationWindow, deltaTime);
        }
        else
        {
            debugCamera.ProcesssInputs(applicationWindow, deltaTime);
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Camera Controll
        if (glfwGetKey(applicationWindow, GLFW_KEY_C) == GLFW_PRESS)
        {
            mainCamera.m_IsMain = !mainCamera.m_IsMain;
            debugCamera.m_IsMain = !debugCamera.m_IsMain;
        }

        // MAIN RENDERING LOGIC
        glCheckError();

        grassRenderer.Render(&debugCamera);

        glCheckError();

        // AFTER DRAW
        glfwSwapBuffers(applicationWindow);

        // FPS Counter
        frameCount++;
        elapsedTime = glfwGetTime() - previousTime;
        if (elapsedTime >= 1.0) // 1 second
        {
            FPS = int(frameCount / elapsedTime);
            previousTime = glfwGetTime(); frameCount = 0;

            glfwSetWindowTitle(applicationWindow, ("Grass Mania | FPS: " + std::to_string(FPS)).c_str());
        }
    }

#pragma region CleanUp

    glfwDestroyWindow(applicationWindow);
    glfwTerminate();

#pragma endregion

    return EXIT_SUCCESS;
}
