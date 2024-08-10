#pragma once

#include "VAO.h"


namespace whim {
	VAO::VAO() {
		glGenVertexArrays(1, &id);
	}

	bool VAO::delete_vao()
	{
		Logger::log("Deleting VAO with id " + std::to_string(id));
		glDeleteVertexArrays(1, &id);
		return true;
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
