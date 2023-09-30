#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
//#include<FreeImage/FreeImagePlus.h>

#include"ShaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// Create points to draw triangle.
/*GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS           //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
};

GLuint indices[] =
{
	0, 3, 5, // Lower Left Triangle.
	3, 2, 4, // Lower Right Triangle.
	5, 4, 1 // Upper Triangle
};*/

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	 // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	 // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	 // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	 // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

int main()
{
	// Specify the window width, height, and name.
	// Will be replaced with arguments in actual program.
	int windowWidth = 800;//1920;
	int windowHeight = 800;//1080;
	const char *windowName = "Game Title";
	// I have no idea how to select the primary monitor right now.
	// Setting this to null keeps the window in windowed mode.
	GLFWmonitor *bestMonitor = NULL;

	// Initialize GLFW.
	glfwInit();

	// Tell GLFW what version of OpenGL we are using.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window object with specified cordinents.
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, bestMonitor, NULL);
	// Make sure the window did not fail to create.
	if (window == NULL) {
		std::cout << "Falled to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Add window icon.
	GLFWimage images[1];
	images[0].pixels = stbi_load("Images/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	// Introduce window to current context.
	glfwMakeContextCurrent(window);

	// Load GLAD so it is configured to OpenGL.
	gladLoadGL();

	// Specify the OpenGL Viewport in the window.
	glViewport(0, 0, windowHeight, windowHeight);

	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Main while loop.
	// This is probably where we will also run update, physics, ect.
	while (!glfwWindowShouldClose(window)) 
	{
		// Specify background color.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clear back buffer and assugn the color to it.
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program to use.
		shaderProgram.Activate();
		glUniform1f(uniID, .5f);
		// Bind the VAO so OpenGL knows how to use it.
		VAO1.Bind();
		// Draw triangle using the GL_TRIANGLES primative.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the current buffer with the back buffer we just edited.
		glfwSwapBuffers(window);

		// Service all GLFW events.
		glfwPollEvents();
	}

	// Trying to add a screenshot feature
	GLubyte* pixels = new GLubyte[3 * windowWidth * windowHeight];

	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Delete all objects created.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// when the program escapes the while loop, destroy the window and terminate GLFW to avoid memory leaks.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}