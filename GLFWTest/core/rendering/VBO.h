#pragma once
#include <glad/glad.h>
#include <vector>

namespace whim {

	class VBO
	{
	private:
		GLuint id;

	public:
		VBO();

		~VBO();

		void bind();

		void unbind() const;

		void setData(const std::vector<float>& data);

		// Specify the vertex attributes
		// https://docs.gl/gl3/glVertexAttribPointer
		void linkAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

	};
}

