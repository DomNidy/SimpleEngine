#include "Camera.hpp"
#include "../logger/Logger.hpp"
#include <string>

namespace whim {

	Camera::Camera()
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		_scene = nullptr;
	}
	glm::mat4 Camera::generate_view_matrix() const
	{
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		return view;
	}

	void Camera::process_input_command(KbdInputCommand command)
	{
		constexpr float moveSpeed = 1.5f;
		constexpr float turnSpeed = 100.0f;

		float delta_time = _scene->get_delta_time();

		switch (command) {
		case MOVE_FWD:
			cameraPos += moveSpeed * delta_time * cameraFront;
			break;
		case MOVE_BACK:
			cameraPos -= moveSpeed * delta_time * cameraFront;
			break;
		case MOVE_RIGHT:
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * delta_time;
			break;
		case MOVE_LEFT:
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * delta_time;
		}

	}
}
