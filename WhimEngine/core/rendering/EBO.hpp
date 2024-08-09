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


		/// <summary>
		/// Set data of this EBO
		/// </summary>
		/// <param name="data">The actual data to bind to EBO</param>
		/// <param name="usage">The nature in which we access this EBO's data, allows GL to perform optimizations.</param>
		void set_data(const std::vector<unsigned int> data, GLenum usage);
		void bind() const;
		void unbind() const;
	};
}

