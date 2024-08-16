#include <iostream>

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>

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
	glfwMakeContextCurrent(applicationWindow);

	if (gl3wInit())
	{
		std::cerr << "ERROR: Failed to Initialize gl3w";
		exit(EXIT_FAILURE);
	}

	if (glfwWindowShouldClose(applicationWindow))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.3, 0.5, 0.8, 1.0);

		glfwSwapBuffers(applicationWindow);
	}

	return EXIT_SUCCESS;
}