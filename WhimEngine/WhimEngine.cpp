#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include "thirdparty/stb_image/stb_image.h"
#include "core/logger/Logger.hpp"
#include "core/input/Input.hpp"
#include "core/asset-loader/AssetLoader.hpp"
#include "core/asset-loader/ShaderAssets.hpp"
#include "core/rendering/Vertex.h"
#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "core/rendering/VBO.h"
#include "core/rendering/EBO.hpp"
#include "core/rendering/VAO.h"
#include "core/rendering/Camera.hpp"
#include "core/game/Scene.h"

using namespace whim;

constexpr double pi = 3.14159265358979323846;
constexpr double e = 2.71828182845904523536;

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

void print_matrix(const glm::mat4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			std::cout << matrix[col][row] << " ";
		}
		std::cout << std::endl;
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

static void glfw_error_callback(int error, const char* description) {
	std::string errorMessage = "CALLBACK Error " + std::to_string(error) + ": " + description;
	whim::Logger::log(errorMessage);
}


static double lastTime = glfwGetTime();
double getDeltaTime() {
	double deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();
	return deltaTime;
}



int main(void)
{

	glfwSetErrorCallback(glfw_error_callback);

	GLFWwindow* window;
	/* Initialize the glfw */
	if (!glfwInit()) {
		Logger::log_error("Failed to initialize glfw.");
		return -1;
	}

	// GLSL (gl shading language) version
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Whim Engine", NULL, NULL);

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup platform/renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// Specify viewport of OpenGL in the window
	glViewport(0, 0, 1920, 1080);

	// Set up clipping plane
	// https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glClipPlane.xml

	// Specify background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clear the color buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	whim::Vertex vertices[] = {
		// Front face
		{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, // bottom right
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 0.0f)}, // bottom left
		{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f)}, // top right
		{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 1.0f)}, // top left

		// Back face
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f)},
		{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.5f)},
		{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f)},

		// Left face
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.0f)},
		{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.5f)},
		{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)},

		// Right face
		{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(0.5f, 0.0f)},
		{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(0.0f, 0.5f)},
		{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)},

		// Top face
		{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.0f)},
		{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.5f)},
		{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f)},

		// Bottom face
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.5f)},
		{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)},
	};

	GLuint indices[] = {
		// Front face indices
		0, 1, 2,
		2, 1, 3,

		// Back face
		4, 5, 6,
		6, 5, 7,

		// Left face
		8, 9, 10,
		10, 9, 11,

		// Right face
		12, 13, 14,
		14, 13, 15,

		// Top face
		16, 17, 18,
		18, 17, 19,

		// Bottom face
		20, 21, 22,
		22, 21, 23
	};

	// Creating VAO and binding it
	// The vao encapsulates vbo and ebo
	whim::VAO cubeVAO;
	cubeVAO.bind();

	// Create vbo
	whim::VBO cubeVBO;
	cubeVBO.setData(std::vector<whim::Vertex>(vertices, vertices + 24));

	// Position attrib pointer
	cubeVAO.setAttributePointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Color attrib pointer
	cubeVAO.setAttributePointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Texcoord attrib pointer
	cubeVAO.setAttributePointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

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

	//* MOVE THIS CODE SOMEWHERE INTO THE ASSET LOADER
	// Loading a texture
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load("assets/textures/grass-block.jpg", &width, &height, &nrChannels, 0);
	if (!textureData) {
		whim::Logger::log_error("Faled to load texture");
	}
	else {
		whim::Logger::log("Loaded texture");
		whim::Logger::log("Texture dimensions: " + std::to_string(width) + "x" + std::to_string(height) + "," + std::to_string(nrChannels));
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);

	// UI state
	bool show_window = true;
	glm::vec3 clear_color = glm::vec3(0.12f, 0.083f, 0.105f);

	float rotX = 0.001f;
	float rotY = 0.001f;
	float rotZ = 0.001f;

	Camera cam = Camera();
	Input input = Input(window);

	Scene scene = Scene();

	scene.register_camera(&cam);
	scene.register_input(&input);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Process inputs/logic
		scene.tick();

		glUseProgram(shaderProgram);

		/*glm::mat4 view = scene.getCamera()->generateViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp = projection * view * model;*/

		// Pass the MVP matrix to the shader
		GLuint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
		//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));


		// Start the dear imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_window) {
			ImGui::Begin("Whim Debugger");

			ImGui::Text("Transform");
			//ImGui::Text(("Matrix" + glm::to_string(mvp)).c_str());

			ImGui::ColorEdit3("clear color", glm::value_ptr(clear_color));

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT);
		// Update the clear color (incase we changed it from the debugger)
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);

		// Imgui rendering (do this before our draw calls)
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Our draw calls
		// https://docs.gl/gl3/glDrawElements
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

