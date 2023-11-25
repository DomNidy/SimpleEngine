#include "VBO.h"
#include "../../core//logger/Logger.hpp"
#include <sstream>
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

	void VBO::setData(const std::vector<float>& data) {
		bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	}

	void VBO::linkAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		bind();
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);
	}

}
