#pragma once

#include "VAO.h"


namespace whim {
	VAO::VAO() {
		glGenVertexArrays(1, &id);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &id);
	}

	void VAO::bind() {
		glBindVertexArray(id);
	}

	void VAO::unbind() const {
		glBindVertexArray(0);
	}

	void VAO::set_attribute_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
}
