#pragma once
#include <map>
#include <string>
#include "../logger/Logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace whim {
	class Input
	{
	public:
		static Input& getInstance() {
			static Input instance;
			return instance;
		}

		static void read_inputs(GLFWwindow* window, int key, int scancode, int action, int mods);
	private:
		explicit Input();
		std::map<int, std::string> input_command_map_;
	};
}

