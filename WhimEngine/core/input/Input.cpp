#include "Input.hpp"


namespace whim {

	void Input::read_inputs(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Logger::log(std::to_string(key));
	}


	Input::Input() {
		Input::input_command_map_.emplace(87, "MOVE_FWD");
	}
}