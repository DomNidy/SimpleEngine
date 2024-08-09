#include "Util.hpp"

namespace whim::Util {

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


	unsigned int compile_shader(unsigned int type, const std::string& source) {
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
	int create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
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

	void glfw_error_callback(int error, const char* description) {
		std::string errorMessage = "CALLBACK Error " + std::to_string(error) + ": " + description;
		whim::Logger::log(errorMessage);
	}

}
