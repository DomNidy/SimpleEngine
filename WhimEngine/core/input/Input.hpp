#pragma once
#include <unordered_map>
#include <string>
#include "../logger/Logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../rendering/Camera.hpp"

namespace whim {
	enum InputCommand {
		MOVE_FWD,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_BACK,
		JUMP
	};

	class Input
	{
	public:
		Input();

		friend class Scene;
	protected:
		void process_input(GLFWwindow* window, Camera* camera, float delta_time, int key, int scancode, int action, int mods);
	private:
		std::unordered_map<int, InputCommand> input_command_map;
	};
}

