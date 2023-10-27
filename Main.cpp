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

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
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
	Shader transparentProgram("default.vert", "transparent.frag");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");
	Shader outliningProgram("outlining.vert", "outlining.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), 0, 5, 0);
	transparentProgram.Activate();
	glUniform4f(glGetUniformLocation(transparentProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(transparentProgram.ID, "lightPos"), 0, 5, 0);
	//framebufferProgram.Activate();
	//glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glDepthFunc(GL_LESS);

	Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 10.0f));

	Model model0("Model/plane/scene.gltf");
	//Model grass("Model/Grass/grass.gltf");

	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string facesCubemap[6] =
	{
		"Skybox/right.jpg",
		"Skybox/left.jpg",
		"Skybox/top.jpg",
		"Skybox/bottom.jpg",
		"Skybox/front.jpg",
		"Skybox/back.jpg",
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// Uncomment to disable VSync
	// glfwSwapInterval(0);

	unsigned int counter = 0;

	// Main while loop.
	// This is probably where we will also run update, physics, ect.
	while (!glfwWindowShouldClose(window) and glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
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

		SetSkyBoxColor(glm::vec4(0.07f, 0.13f, 0.17f, 1.0f));
		// Clear back and depth buffer and stencil buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Handle camera inputs, will probably detach this from the camera itself eventually.
		camera.Inputs(window);

		// Update and export camera matrix to vertex shader.
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		model0.Draw(shaderProgram, camera);

		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.position, camera.position + camera.orientation, camera.up)));
		projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

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
