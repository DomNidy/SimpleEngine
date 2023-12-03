#pragma once

#include "VAO.h"


namespace whim {
	VAO::VAO() {
		glGenVertexArrays(1, &id);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &id);
	}

	void VAO::bind() {
		glBindVertexArray(id);
	}

	void VAO::unbind() const {
		glBindVertexArray(0);
	}


}
