#include "Input.hpp"
#include <glm.hpp>

namespace whim {
	Input::Input(GLFWwindow* window) {
		kbd_input_command_map[MOVE_FWD] = GLFW_KEY_W;
		kbd_input_command_map[MOVE_LEFT] = GLFW_KEY_A;
		kbd_input_command_map[MOVE_BACK] = GLFW_KEY_S;
		kbd_input_command_map[MOVE_RIGHT] = GLFW_KEY_D;
		this->window = window;
		this->_scene = nullptr;
	}

	Input::Input() : window(nullptr), _scene(nullptr) {
		kbd_input_command_map[MOVE_FWD] = GLFW_KEY_W;
		kbd_input_command_map[MOVE_LEFT] = GLFW_KEY_A;
		kbd_input_command_map[MOVE_BACK] = GLFW_KEY_S;
		kbd_input_command_map[MOVE_RIGHT] = GLFW_KEY_D;
	}

	void Input::process_input()
	{
		process_input_mouse();

		// TODO: Refactor this notify method to use command pattern
		// We are just calling the overload which rotates the camera
		notify_observers(delta_pos.x, delta_pos.y);

		// Check if W key was pressed
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_FWD]) == GLFW_PRESS) {
			notify_observers(MOVE_FWD);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_LEFT]) == GLFW_PRESS) {
			notify_observers(MOVE_LEFT);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_RIGHT]) == GLFW_PRESS) {
			notify_observers(MOVE_RIGHT);
		}
		if (glfwGetKey(this->window, kbd_input_command_map[MOVE_BACK]) == GLFW_PRESS) {
			notify_observers(MOVE_BACK);
		}
	}

	// TODO: Refactor to command pattern
	void Input::notify_observers(KbdInputCommand command)
	{
		for (Camera* cam : observers) {
			cam->process_input_command(command);
		}
	}

	// TODO: Refactor to command pattern
	void Input::notify_observers(float x_offset, float y_offset) {
		for (Camera* cam : observers) {
			cam->process_mouse_movement(x_offset, -y_offset);
		}
	}

	void Input::process_input_mouse()
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		delta_pos.x = xpos - current_pos.x;
		delta_pos.y = ypos - current_pos.y;

		current_pos.x = xpos;
		current_pos.y = ypos;
	}

	void Input::register_observer(Camera* camera) {
		this->observers.push_back(camera);
	}

	void Input::unregister_observer(Camera* camera) {
		this->observers.erase(std::remove(observers.begin(), observers.end(), camera));
	}
}