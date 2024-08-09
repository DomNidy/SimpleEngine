#include "EBO.hpp"

namespace whim {

	EBO::EBO() {
		glGenBuffers(1, &id);
	}

	EBO::~EBO() {
		glDeleteBuffers(1, &id);
	}

	void EBO::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void EBO::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void EBO::set_data(const std::vector<unsigned int> data, GLenum usage) {
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), usage);
	}
}
