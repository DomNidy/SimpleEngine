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



	};
}