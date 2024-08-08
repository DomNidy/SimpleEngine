#include "Camera.hpp"

namespace whim {

	Camera::Camera()
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	glm::mat4 Camera::generateViewMatrix() const
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
		case MOVE_BACK:
			cameraPos -= moveSpeed * delta_time * cameraFront;
		case MOVE_RIGHT:
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * delta_time;
		case MOVE_LEFT:
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * delta_time;
		}

	}
}
