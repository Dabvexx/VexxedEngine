#include"Model.h"
//#include<FreeImage/FreeImagePlus.h>


void SetSkyBoxColor(glm::vec4 skyBoxColor)
{
	// Specify background color.
	glClearColor(skyBoxColor.x, skyBoxColor.y, skyBoxColor.z, skyBoxColor.w);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void CalculateDeltaTime() 
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

bool InitGL(GLFWwindow* window)
{
	// Put all the setup and window code in here, also probably return that window or have it as an out.
		// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Tell GLFW what version of OpenGL we are using.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window = glfwCreateWindow(windowWidth, windowHeight, windowName, bestMonitor, NULL);

	glEnable(GL_DEPTH_TEST);
}

// Make function that will cap framerate as well as calculate delta time.
/*bool FixedTimeStep(int fps, float& outDeltaTime)
{
	// Needs to happen before the function call, will fix later
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// simple timer.
	double crntTime = glfwGetTime();
	if (crntTime - prevTime >= 1 / 60)
	{
		prevTime = crntTime;
	}
}*/

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

	/*if (!InitGL())
	{
		std::cerr << "Failed to initialize core function" << std::endl;
		return -1;
	}*/

	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Tell GLFW what version of OpenGL we are using.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Create a window object with specified cordinents.
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, bestMonitor, NULL);
	// Make sure the window did not fail to create.
	if (window == NULL) {
		std::cerr << "Falled to create GLFW window" << std::endl;
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

	Shader outliningProgram("outlining.vert", "outlining.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), 0, 5, 0);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glDepthFunc(GL_LESS);

	Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model0("Model/test/test.gltf");

	unsigned int counter = 0;

	// Uncomment to disable VSync
	// glfwSwapInterval(0);

	// Main while loop.
	// This is probably where we will also run update, physics, ect.
	while (!glfwWindowShouldClose(window) and glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		SetSkyBoxColor(glm::vec4(0.07f, 0.13f, 0.17f, 1.0f));

		// Clear back and depth buffer and stencil buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// CalculateDeltaTime();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		counter++;

		// Calculate fps and ms to calculate frame.
		if (deltaTime >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) / 1000);
			std::string newTitle = "Vexxed Engine - " + FPS + "Fps / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			lastFrame = currentFrame;
			counter = 0;
		}

		// Handle camera inputs, will probably detach this from the camera itself eventually.
		camera.Inputs(window);

		// Update and export camera matrix to vertex shader.
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		// Set stencil buffer parameters.
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		model0.Draw(shaderProgram, camera);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		
		// draw outlining program.
		outliningProgram.Activate();
		glUniform1f(glGetUniformLocation(outliningProgram.ID, "outlining"), 1.08f);
		//model0.Draw(outliningProgram, camera);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// Swap the current buffer with the back buffer we just edited.
		glfwSwapBuffers(window);

		// Service all GLFW events.
		glfwPollEvents();
	}

	// Trying to add a screenshot feature
	GLubyte* pixels = new GLubyte[3 * windowWidth * windowHeight];

	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// when the program escapes the while loop, destroy the window and terminate GLFW to avoid memory leaks.
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
