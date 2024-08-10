#pragma once
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"
#include "../logger/Logger.hpp"

namespace whim {

	class VBO
	{
		

	public:
		/// <summary>
		/// Create a new VBO with the specified target
		/// </summary>
		/// <param name="target">target we will bind this buffer to</param>
		VBO(GLenum target);

		/// <summary>
		/// Create a new VBO
		/// Target is implicitly set to GL_ARRAY_BUFFER if none is specified
		/// </summary>
		VBO();

		// Opengl assigned id/name
		GLuint id;

		/// <summary>
		/// Deletes the data stored in the buffer & allows it's name to be assigned to a new buffer
		/// Returns true upon successful deletion
		/// </summary>
		bool delete_buffer();

		/// <summary>
		/// Binds the buffer to it's target
		/// </summary>
		void bind() const;

		/// <summary>
		/// Unbinds the buffer from it's target
		/// </summary>
		void unbind() const;

		/// <summary>
		/// Creates a new data store for the buffer associated with this VBO. If one exists, it is overwritten.
		/// The size of the created data store is determined by the size of the passed data object.
		/// </summary>
		/// <param name="data">The data this buffer should store</param>
		/// <param name="usage">hints to the GL implementation how this buffer object's data store will be accessed
		/// allows for optimizations</param> 
		/// 
		/// From: https://docs.gl/gl3/glBufferData
		/// `usage` can be broken down into two parts: first, the frequency of access,
		/// and second, the nature of that access.
		/// 
		/// The frequency of access may be one of these
		/// 
		/// STREAM
		///		The data store contents will be modified once and used at most a few times.
		///
		///	STATIC
		///		The data store contents will be modified once and used many times.
		///
		///	DYNAMIC
		///		The data store contents will be modified repeatedly and used many times.
		/// 
		/// The nature of access may be one of these:
		///
		/// DRAW
		///		The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
		///
		///	READ
		///		The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
		///
		///	COPY
		///		The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
		void set_data(const std::vector<whim::Vertex>& data, GLenum usage) const;
		void set_data(const std::vector<glm::mat4>& data, GLenum usage) const;
		// Log out the data in the vbo
		void log_vbo_data() const;
	private:
		/// <summary>
		/// Target that this VBO will bind to
		/// The target determines how OpenGL interprets the data stored in the buffer
		/// </summary>
		GLenum target;
	};
}

