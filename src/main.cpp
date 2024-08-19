#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Camera.h"

int main()
{
#pragma region Initialization

    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed to initialize GLFW";
        exit(EXIT_FAILURE);
    }

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

    // Settings
    glEnable(GL_DEPTH_TEST);

#pragma endregion

    glm::vec3 cameraPosition = { -1.0f, 0.0f, 0.0f };
    glm::vec3 targetToLook   = { 0.0f, 0.0f, 0.0f };
    Camera camera(cameraPosition, targetToLook);

    Mesh grass("./resources/models/grass.obj");

    while (!glfwWindowShouldClose(applicationWindow))
    {
        // PRE DRAW
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
        // MAIN RANDERING LOGIC
        ShaderProgram mainShaderProgram("MainShader", "./resources/shaders/MainVertex.glsl", "./resources/shaders/MainFragment.glsl");

        mainShaderProgram.Bind();

        glm::mat4 modelMatrix(1.0f);
        mainShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

        glm::mat4 viewMatrix = camera.getViewMatrix();
        mainShaderProgram.setMat4("u_ViewMatrix", viewMatrix);

        glm::mat4 projectionMatrix = camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);
        mainShaderProgram.setMat4("u_ProjectionMatrix", projectionMatrix);

        grass.Render(mainShaderProgram);  

        mainShaderProgram.Unbind();

        // AFTER DRAW
        glfwSwapBuffers(applicationWindow);
    }

#pragma region CleanUp

    glfwDestroyWindow(applicationWindow);
    glfwTerminate();

#pragma endregion

    return EXIT_SUCCESS;
}
