#include "Input.hpp"
#include "../game/Scene.h"
#include <glm.hpp>


namespace whim {




	Input::Input() {
		Input::input_command_map[GLFW_KEY_W] = InputCommand::MOVE_FWD;
		Input::input_command_map[GLFW_KEY_A] = InputCommand::MOVE_LEFT;
		Input::input_command_map[GLFW_KEY_S] = InputCommand::MOVE_BACK;
		Input::input_command_map[GLFW_KEY_D] = InputCommand::MOVE_RIGHT;
	}
	void Input::process_input(GLFWwindow* window,  Camera* camera, float delta_time, int key, int scancode, int action, int mods)
	{
		// getDeltaTime() is a non const method, but we have a const Scene pointer
		// meaning we cant call any methods that are non const
		constexpr float moveSpeed = 1.5f;
		constexpr float turnSpeed = 100.0f;

		// Get input for movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->cameraPos += moveSpeed * delta_time * camera->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->cameraPos -= moveSpeed * delta_time * camera->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * moveSpeed * delta_time;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * moveSpeed * delta_time;
	}
}