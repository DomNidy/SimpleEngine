#include "VBO.h"

namespace whim {
	VBO::VBO(GLenum target) {
		this->target = target;
		glGenBuffers(1, &id);
	}

	VBO::VBO() {
		this->target = GL_ARRAY_BUFFER;
		glGenBuffers(1, &id);
	}


	bool VBO::delete_buffer() {
		Logger::log("Deleting VBO with id " + std::to_string(id));
		glDeleteBuffers(1, &id);
		return true;
	}

	void VBO::bind() const
	{
		glBindBuffer(target, id);
	}

	void VBO::unbind() const {
		glBindBuffer(target, 0);
	}

	void VBO::set_data(const std::vector<whim::Vertex>& data, GLenum usage) const {
		bind();
		glBufferData(target, data.size() * sizeof(whim::Vertex), data.data(), usage);
	}

	// TODO: Implement a set_data function that uses generics
	void VBO::set_data(const std::vector<glm::mat4>& data, GLenum usage) const {
		bind();
		glBufferData(target, data.size() * sizeof(glm::mat4), data.data(), usage);
	}

	void VBO::log_vbo_data() const {
		bind();

		// Get size of the VBO data
		GLint dataSize;
		glGetBufferParameteriv(target, GL_BUFFER_SIZE, &dataSize);

		std::vector<float> data(dataSize / sizeof(float));

		glGetBufferSubData(target, 0, dataSize, data.data());
		for (size_t i = 0; i < data.size(); i += 8) {
			whim::Logger::log("Vertex " + std::to_string(i / 8) + ": ");
			whim::Logger::log("Position=(" + std::to_string(data[i]) + ", " + std::to_string(data[i + 1]) + ", " + std::to_string(data[i + 2]) + ")");
			whim::Logger::log("Color=(" + std::to_string(data[i + 3]) + ", " + std::to_string(data[i + 4]) + ", " + std::to_string(data[i + 5]) + ")");
			whim::Logger::log("TexCoord=(" + std::to_string(data[i + 6]) + ", " + std::to_string(data[i + 7]) + ")");
		}
	}

}
