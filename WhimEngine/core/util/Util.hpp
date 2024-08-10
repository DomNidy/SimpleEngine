#pragma once
#include <glad/glad.h>
#include "../logger/Logger.hpp"
#include <string>
#include <glm.hpp>

namespace whim::Util {

	void check_shader_compilation(unsigned int shader, const char* shaderType);
	void check_program_linking(unsigned int program);
	unsigned int compile_shader(unsigned int type, const std::string& source);
	int create_shader(const std::string& vertexShader, const std::string& fragmentShader);
	void glfw_error_callback(int error, const char* description);

	void print_mat4(const glm::mat4& matrix);
}