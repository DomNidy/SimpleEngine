#pragma once
#include <vector>
#include <glad/glad.h>
#include "../../core//logger/Logger.hpp"
#include <sstream>

namespace whim {
	class VAO {
		

	public:
		/// <summary>
		/// A VAO is an OpenGL object that stores the state of a set of vertex arrays, 
		/// including the vertex data, indices, and other relevant information.
		/// </summary>
		VAO();

		bool delete_vao();

		void bind();

		void unbind() const;

		/// <summary>
		/// Specify the format and location of the vertex data in the currently bound VBO
		/// This will determine how shader programs read the vertex data
		/// https://docs.gl/gl3/glVertexAttribPointer
		/// </summary>
		void set_attribute_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
		
		/// <summary>
		/// Id assigned to this vao by opengl (name)
		/// </summary>
		GLuint id;
	};
}