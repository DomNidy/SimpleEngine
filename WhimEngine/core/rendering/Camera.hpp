#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace whim {
	class Camera
	{

	public:
		Camera();

		/// <summary>
		/// Computes the view matrix based on the cameras state, which is used to transform
		/// vertices from world space to camera space.
		/// </summary>
		/// <returns></returns>
		glm::mat4 generateViewMatrix();

		// Store current position of camera in world space
		glm::vec3 cameraPos;

		/// <summary>
		/// Points in the direction that the camera is looking. Used to compute the direction from
		/// the camera's position to the target point. By updating this vector based on user input,
		/// (e.g., mouse movement), you can change the direction the camera is facing.
		/// </summary>
		glm::vec3 cameraFront;

		/// <summary>
		/// This vector defines the upward direction relative to the camera. It helps in keeping 
		/// the camera properly oriented (i.e., preventing it from flipping upside down).
		/// It is usually set to the world's up direction .
		/// </summary>
		glm::vec3 cameraUp;

		float yaw = -90.f;
		float pitch = 0.0f;

		float lastX = 800.0f / 2.0f;
		float lastY = 600.0f / 2.0f;
		bool firstMouse = true;
	};

}

