#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thirdparty/stb_image/stb_image.h"
#include "core/logger/Logger.hpp"
#include "core/input/Input.hpp"
#include "core/asset-loader/AssetLoader.hpp"
#include "core/asset-loader/ShaderAssets.hpp"
#include <glm.hpp>
#include "core/rendering/VBO.h"
#include "core/rendering/EBO.hpp"
#include "core/rendering/VAO.h"

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
enum Rotations {
	X,
	Y,
	Z
};
void rotate(unsigned int shaderProgram, float rotAmount, Rotations rotDir) {
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

	switch (rotDir) {
	case X:
		glUniformMatrix3fv(loc, 1, false, X_rotMatrix);
		break;
	case Y:
		glUniformMatrix3fv(loc, 1, false, Y_rotMatrix);
		break;
	case Z:
		glUniformMatrix3fv(loc, 1, false, Z_rotMatrix);
		break;
	default:
		whim::Logger::log_error("No rotation matrix was specified, unable to transform verticies");
		break;
	}
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

		if (message != nullptr) {
			glGetShaderInfoLog(id, length, &length, message);
			Logger::log_error(message);
		}
		else {
			Logger::log_error("Failed to allocate memory for shader compilation error info log.");
		}

		Logger::log_error("Failed to compile " + (type == GL_VERTEX_SHADER ? std::string("vertex") : std::string("fragment")) + " shader!");
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

	float vertices[] = {
		// Vertices positions          // Color values          // Texture coords
		// Front face
		-0.5f, -0.5f,  0.5f,          1.0f, 0.0f, 1.0f,       0.0f, 0.0f, // Bottom Left 0
		 0.5f, -0.5f,  0.5f,          1.0f, 0.0f, 1.0f,       1.0f, 0.0f, // Bottom Right 1
		-0.5f,  0.5f,  0.5f,          1.0f, 0.0f, 1.0f,       0.0f, 1.0f, // Top Left 2
		 0.5f,  0.5f,  0.5f,          1.0f, 0.0f, 1.0f,       1.0f, 1.0f, // Top Right 3

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,         1.0f, 0.5f, 1.0f,       0.0f, 0.0f, // Bottom Left 4
		  0.5f, -0.5f, -0.5f,         1.0f, 0.5f, 1.0f,       1.0f, 0.0f, // Bottom Right 5
		 -0.5f, -0.5f,  0.5f,         1.0f, 0.5f, 1.0f,       0.0f, 1.0f, // Top Left 6
		  0.5f, -0.5f,  0.5f,         0.0f, 0.5f, 1.0f,       1.0f, 1.0f, // Top Right 7

		  // Back face
		   0.5f, -0.5f, -0.5f,         0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // Bottom Left 8
		  -0.5f, -0.5f, -0.5f,         0.0f, 0.0f, 1.0f,       1.0f, 0.0f, // Bottom Right 9
		   0.5f,  0.5f, -0.5f,         0.0f, 0.0f, 1.0f,       0.0f, 1.0f, // Top Left 10
		  -0.5f,  0.5f, -0.5f,         0.0f, 0.0f, 1.0f,       1.0f, 1.0f, // Top Right 11

		  // Left face
		  -0.5f, -0.5f, -0.5f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f, // Bottom Left 12
		  -0.5f, -0.5f,  0.5f,         1.0f, 1.0f, 1.0f,       1.0f, 0.0f, // Bottom Right 13
		  -0.5f,  0.5f, -0.5f,         1.0f, 1.0f, 1.0f,       0.0f, 1.0f, // Top Left 14
		  -0.5f,  0.5f,  0.5f,         1.0f, 1.0f, 1.0f,       1.0f, 1.0f, // Top Right 15

		  // Right face
		   0.5f, -0.5f,  0.5f,         0.0f, 1.0f, 1.0f,       0.0f, 0.0f, // Bottom Left 16
		   0.5f, -0.5f, -0.5f,         0.0f, 1.0f, 1.0f,       1.0f, 0.0f, // Bottom Right 17
		   0.5f,  0.5f,  0.5f,         0.0f, 1.0f, 1.0f,       0.0f, 1.0f, // Top Left 18
		   0.5f,  0.5f, -0.5f,         0.0f, 1.0f, 1.0f,       1.0f, 1.0f, // Top Right 19

		   // Top face
		   -0.5f,  0.5f,  0.5f,         0.5f, 0.0f, 1.0f,       0.0f, 0.0f, // Bottom Left 20
			0.5f,  0.5f,  0.5f,         0.5f, 0.0f, 1.0f,       1.0f, 0.0f, // Bottom Right 21
		   -0.5f,  0.5f, -0.5f,         0.5f, 0.0f, 1.0f,       0.0f, 1.0f, // Top Left 22
			0.5f,  0.5f, -0.5f,          0.5f, 0.0f, 1.0f,       1.0f, 1.0f, // Top Right 23
	};

	unsigned int indices[] = {
		// Front face indices
		0, 2, 1,
		1, 2, 3,

		// Bottom face indices
		4, 5, 6,
		5, 7, 6,

		// Back face indices
		8, 10, 9,
		9, 10, 11,

		// Left face indices
		12, 14, 13,
		13, 14, 15,

		// Right face indices
		16, 18, 17,
		17, 18, 19,

		// Top face indices
		20, 22, 21,
		21, 22, 23,
	};



	// Creating VAO and binding it
	whim::VAO cubeVAO;
	cubeVAO.bind();

	// Create vbo
	whim::VBO cubeVBO;
	cubeVBO.setData(std::vector<float>(vertices, vertices + 192));
	// Position attrib pointer
	cubeVBO.linkAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Color attrib pointer
	cubeVBO.linkAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Texcoord attrib pointer
	cubeVBO.linkAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Create ebo
	whim::EBO cubeEBO;
	cubeEBO.setData(std::vector<unsigned int>(indices, indices + sizeof(indices) / sizeof(indices[0])));


	// Creating and compiling shaders
	unsigned int shaderProgram = create_shader(whim::Assets::Shaders::default_vertex.shader_string,
		whim::Assets::Shaders::default_fragment.shader_string);

	// Specify shader program
	glUseProgram(shaderProgram);

	// Set background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Loading a texture
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load("test_texture.png", &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);

	double lastTime = glfwGetTime();
	double rotSpeed = 5;
	double currentTime = 0.0;
	double deltaTime = 0.0;

	glfwSetKeyCallback(window, Input::read_inputs);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);
		rotate(shaderProgram, currentTime, Rotations::Y);

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

