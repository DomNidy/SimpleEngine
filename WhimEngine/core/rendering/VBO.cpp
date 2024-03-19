#include "VBO.h"

namespace whim {
	VBO::VBO() {
		glGenBuffers(1, &id);
	}

	VBO::~VBO() {
		glDeleteBuffers(1, &id);
	}

	void VBO::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VBO::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::setData(const std::vector<whim::Vertex>& data) {
		bind();
		// Gl buffer data expects size to be in bytes
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(whim::Vertex), data.data(), GL_STATIC_DRAW);
	}

	void VBO::logVBOData() {
		bind();

		// Get size of the VBO data
		GLint dataSize;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &dataSize);

		std::vector<float> data(dataSize / sizeof(float));

		glGetBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data.data());
		for (size_t i = 0; i < data.size(); i += 8) {
			whim::Logger::log("Vertex " + std::to_string(i / 8) + ": ");
			whim::Logger::log("Position=(" + std::to_string(data[i]) + ", " + std::to_string(data[i + 1]) + ", " + std::to_string(data[i + 2]) + ")");
			whim::Logger::log("Color=(" + std::to_string(data[i + 3]) + ", " + std::to_string(data[i + 4]) + ", " + std::to_string(data[i + 5]) + ")");
			whim::Logger::log("TexCoord=(" + std::to_string(data[i + 6]) + ", " + std::to_string(data[i + 7]) + ")");

		}
	}

}
