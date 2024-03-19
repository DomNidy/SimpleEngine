#pragma once
#include <vector>
#include <glad/glad.h>
#include "../../core//logger/Logger.hpp"
#include <sstream>

namespace whim {
	class VAO {
	private:
		GLuint id;

	public:
		VAO();
		~VAO();

		void bind();

		void unbind() const;

		// Specify the vertex attribute pointers
		// https://docs.gl/gl3/glVertexAttribPointer
		void setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

	};
}