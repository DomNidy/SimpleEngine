#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thirdparty/stb_image/stb_image.h"
#include "core/logger/Logger.hpp"
#include "core/asset-loader/AssetLoader.hpp"
#include "core/asset-loader/ShaderAssets.hpp"

using namespace whim;

void check_shader_compilation(unsigned int shader, const char* shaderType)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::string logErrorStr = shaderType;
		logErrorStr += "::COMPILATION_FAILED\n";
		Logger::log_error(logErrorStr);
		Logger::log_error(infoLog);
	}
}

void check_program_linking(unsigned int program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		Logger::log_error("ERROR::SHADER_PROGRAM::LINKING_FAILED\n");
		Logger::log_error(infoLog);
	}
}

void moveCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Logger::log(std::to_string(key));
}

int main(void)
{

	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit()) {
		Logger::log_error("Failed to initialize glfw.");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);

	if (!window)
	{
		Logger::log_error("Failed to create window");
		glfwTerminate();
		return -1;
	}


	// Make the window's context current 
	glfwMakeContextCurrent(window);

	// Load glad so it configures open gl (generates code from gpu driver functions)
	// Note: It is important that this is called only after we run `glfwMakeContextCurrent()` on our window
	// Glad will load the gl functions FOR THIS CONTEXT
	if (gladLoadGL()) {
		Logger::log("Successfully loaded glad functions");
	}
	else {
		Logger::log_error("Failed to load glad functions");
	}
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	// Specify viewport of OpenGL in the window
	glViewport(0, 0, 800, 800);
	// Specify background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clear the color buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// These verticies will be
	// put into the vbo and sent to vertex shader
	float verticies[] = {
		// Position				// Color values
		-0.5f, -0.5f, 0.0f,		0.25f, 0.5f, 1,
		0.5f, -0.5f, 0.0f,		0.25f, 0.5f, 1,
		-0.5f, 0.5f, 0.0f,		0.25f, 0.5f, 1,


		0.5f, -0.5f, 0.0f,		0.25f, 0.5f, 1,
		0.5f, 0.5f, 0.0f,		0.25f, 0.5f, 1,
		-0.5f, 0.5f, 0.0f,		0.25f, 0.5f, 1
	};

	// Creating VAO and binding it
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	// Specify the vertex attributes
	// https://docs.gl/gl3/glVertexAttribPointer

	// Position attrib pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attrib pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Creating and compiling shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &whim::Assets::Shaders::default_vertex.shader_string, NULL);
	glCompileShader(vertexShader);
	check_shader_compilation(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &whim::Assets::Shaders::default_fragment.shader_string, NULL);
	glCompileShader(fragmentShader);
	check_shader_compilation(fragmentShader, "FRAGMENT");

	// Creating shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	check_program_linking(shaderProgram);

	// Delete shaders as they are now linked and no longer needed
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// Specify shader program
	glUseProgram(shaderProgram);

	// Set background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glfwSetKeyCallback(window, moveCallback);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

