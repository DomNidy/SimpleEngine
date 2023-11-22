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

void rotate(unsigned int shaderProgram, float rotAmount) {
	glUseProgram(shaderProgram);
	unsigned int loc = glGetUniformLocation(shaderProgram, "u_Rot");

	float X_rotMatrix[] = {
		// Position				
		1, 0, 0,
		0, cos(rotAmount), -sin(rotAmount),
		-0.5f, sin(rotAmount), cos(rotAmount)
	};

	float Y_rotMatrix[] = {
		cos(rotAmount), 0, sin(rotAmount),
		0, 1, 0,
		-sin(rotAmount), 0, cos(rotAmount)
	};

	float Z_rotMatrix[] = {
		cos(rotAmount), -sin(rotAmount), 0,
		sin(rotAmount), cos(rotAmount), 0,
		0, 0, 1
	};

	// glUniformMatrix3fv(loc, 1, false, X_rotMatrix);
	glUniformMatrix3fv(loc, 1, false, Y_rotMatrix);
	// glUniformMatrix3fv(loc, 1, false, Z_rotMatrix);
}

void move_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Logger::log(std::to_string(key));
}

static unsigned int compile_shader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// TODO: Handle errors here
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		Logger::log_error("Failed to compile " + (type == GL_VERTEX_SHADER ? std::string("vertex") : std::string("fragment")) + " shader!");
		Logger::log_error(message);
		glDeleteShader(id);
	}

	return id;
}

// Returns id of the shader program created
static int create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

	// Attatch shader to shader program & link program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Free memory allocated to shader as they've been linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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
	unsigned int shaderProgram = create_shader(whim::Assets::Shaders::default_vertex.shader_string,
		whim::Assets::Shaders::default_fragment.shader_string);

	// Specify shader program
	glUseProgram(shaderProgram);

	// Set background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	float loops = 0;
	glfwSetKeyCallback(window, move_callback);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		rotate(shaderProgram, loops / 100);


		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
		loops += 0.5;
	}

	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

