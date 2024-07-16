#pragma once

#include <string>

class Shader
{
public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();
};

