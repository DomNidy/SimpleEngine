#pragma once
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"
#include "../logger/Logger.hpp"

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

		void setData(const std::vector<whim::Vertex>& data);

		// Log out the data in the vbo
		void logVBOData();
	};
}

