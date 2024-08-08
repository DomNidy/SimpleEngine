#include "Input.hpp"
#include <glm.hpp>

namespace whim {
	Input::Input(GLFWwindow* window) {
		kbd_input_command_map[MOVE_FWD] = GLFW_KEY_W;
		kbd_input_command_map[MOVE_LEFT] = GLFW_KEY_A;
		kbd_input_command_map[MOVE_BACK] = GLFW_KEY_S;
		kbd_input_command_map[MOVE_RIGHT] = GLFW_KEY_D;
		this->window = window;
	}

	Input::Input() : window(nullptr) {}

	void Input::process_input()
	{

		// Check if W key was pressed
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_FWD]) == GLFW_PRESS) {
			notify_observers(MOVE_FWD);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_LEFT]) == GLFW_PRESS) {
			notify_observers(MOVE_FWD);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_RIGHT]) == GLFW_PRESS) {
			notify_observers(MOVE_RIGHT);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_BACK]) == GLFW_PRESS) {
			notify_observers(MOVE_BACK);
		}
	}

	void Input::notify_observers(KbdInputCommand command)
	{
		for (Camera* cam : this->observers) {
			cam->process_input_command(command);
		}
	}

	void Input::register_observer(Camera* camera) {
		this->observers.push_back(camera);
	}

	void Input::unregister_observer(Camera* camera) {
		this->observers.erase(std::remove(observers.begin(), observers.end(), camera));
	}
}