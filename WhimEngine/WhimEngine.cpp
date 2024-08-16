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
#include "core/rendering/EBO.hpp"
#include "core/rendering/VBO.h"
#include "core/rendering/VAO.h"
#include "core/rendering/Camera.hpp"
#include "core/game/Scene.h"
#include "core/util/Util.hpp"
#include "core/rendering/Voxel.h"

using namespace whim;

constexpr double pi = 3.14159265358979323846;
constexpr double e = 2.71828182845904523536;


int main(void)
{
	glfwSetErrorCallback(Util::glfw_error_callback);

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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int monitor_width, monitor_height;
	glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &monitor_width, &monitor_height);

	Logger::log("Dimensions of primary monitor: " + std::to_string(monitor_width) + "x" + std::to_string(monitor_height));

	GLFWwindow* window;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(monitor_width, monitor_height, "Whim Engine", NULL, NULL);

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


	std::vector<GLuint> indices = {
		// Front face indices
		0, 1, 2,
		2, 3, 1,

		// Back face
		4, 5, 6,
		6, 7, 5,

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


	// TODO: Review these links
	// https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/voxelEngine/src/VoxelGeometry.cpp
	// https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/voxelEngine/src/VoxelRenderer.cpp

	const int CHUNK_W = 1, CHUNK_H = 1, CHUNK_D = 1;

	const GLuint NUM_MESHES = CHUNK_W * CHUNK_H * CHUNK_D;
	VAO mesh_vaos[NUM_MESHES];
	GLuint mesh_vertex_count[NUM_MESHES];
	for (int i = 0; i < CHUNK_W; i++) {
		for (int j = 0; j < CHUNK_H; j++) {
			for (int k = 0; k < CHUNK_D; k++) {
				int curr_vao_index = (CHUNK_W * CHUNK_H * i) + (CHUNK_H * j) + k;
				Voxel vox = Voxel(i, j, k);

				VAO _vao = VAO();
				_vao.bind();
				mesh_vaos[i] = _vao;

				VBO _vbo = VBO();

				_vbo.set_data(vox.verts, GL_STATIC_DRAW);
				mesh_vertex_count[curr_vao_index] = vox.verts.size();

				// Potential Issue: We are using the same index buffer for all VAOs,
				// but we're not taking into account the offset of the vertex data in each vao
				EBO _ebo = EBO();
				_ebo.set_data(indices, GL_STATIC_DRAW);

				Logger::log(std::to_string(curr_vao_index) + " " + std::to_string(i) + " " + std::to_string(j));
				// TODO: Maybe our ebo needs to have an attribute pointer?
				// Position attrib pointer
				mesh_vaos[curr_vao_index].set_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
				// Color attrib pointer
				mesh_vaos[curr_vao_index].set_attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				// Texcoord attrib pointer
				mesh_vaos[curr_vao_index].set_attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			}
		}
	}

	// Creating and compiling shaders
	unsigned int shaderProgram = Util::create_shader(whim::Assets::Shaders::default_vertex.shader_string,
		whim::Assets::Shaders::default_fragment.shader_string);

	// Specify shader program
	glUseProgram(shaderProgram);
	// Set background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// TODO: Memory leak issue here most likely
	// We are create an ImageTextureData struct inside the function's stack frame, and returning a COPY of it
	// This automatically calls the destructor for the struct that was made in the stack frame, but this destructor does not
	// free the image data which is heap allocated.
	// should result in an additional ~16kb of memory being used ?
	ImageTextureData texture_data = whim::AssetLoader::load_texture_data_from_image("assets/textures/grass-block.jpg");
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	texture_data.free_image();

	// UI state
	bool show_window = true;
	glm::vec3 clear_color = glm::vec3(0.12f, 0.083f, 0.105f);

	// Read inputs from window
	Scene scene = Scene();
	Input input = Input(window);
	Camera cam = Camera();

	// cam will respond to inputs from input
	input.register_observer(&cam);

	scene.register_camera(&cam);
	scene.register_input(&input);

	// This compares the z coordinate of each pixel of a rendered object with the value stored in the depth buffer
	// By default, if the incoming depth is less than the stored depth, we'll render (it's closer to the cam)
	// OpenGL will use the clip space projection's z coordinate to test depth.
	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Process inputs/logic
		scene.tick();

		// Shader programs are bound to the current opengl context
		glUseProgram(shaderProgram);

		glm::mat4 view = scene.get_camera()->generate_view_matrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp = projection * view * model;

		// Pass the MVP matrix to the shader
		GLuint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Update the clear color (incase we changed it from the debugger)
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);

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

		// Imgui rendering (do this before our draw calls)
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Our draw calls
		// https://docs.gl/gl3/glDrawElements
		for (int i = 0; i < NUM_MESHES; i++) {
			//Logger::log("Binding " + std::to_string(mesh_vaos[i].id) + " with " + std::to_string(mesh_vertex_count[i]) + " verts.");
			glBindVertexArray(mesh_vaos[i].id);
			// TODO: Read https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
			// TODO: When drawing multiple errors, nvoglv64.dll throws an error,
			// this likely means the data for that element has not been sent to the gpu, or our offset (void*)0 is incorrect
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
			//Sleep(25);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

