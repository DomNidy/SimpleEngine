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
		// This specifies that we SHOULD use the attribute in rendering
		// For example, if we set attribute pointer for color to 2, and enable 2, we will use it when rendering
		// similarly, disabling the attribute with index of 2 would disable color in rendering.
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
}
