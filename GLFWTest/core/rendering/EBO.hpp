#pragma once
#include <vector>
#include <glad/glad.h>

namespace whim {
	class EBO
	{
	private:
		GLuint id;
	public:
		EBO();
		~EBO();
		void setData(const std::vector<unsigned int> data);
		void bind();
		void unbind() const;
	};
}

