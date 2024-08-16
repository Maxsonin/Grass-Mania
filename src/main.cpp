#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed to initialize GLFW";
        exit(EXIT_FAILURE);
    }

    const unsigned int WINDOW_WIDTH = 1000;
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

    while (!glfwWindowShouldClose(applicationWindow))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        glfwSwapBuffers(applicationWindow);
    }

    glfwDestroyWindow(applicationWindow);
    glfwTerminate();

    return EXIT_SUCCESS;
}
