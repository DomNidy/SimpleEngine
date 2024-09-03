#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../game/World.h"
#include "../input/InputCommand.hpp"

namespace whim {

	class World;

	class Camera
	{

	public:
		Camera();

		/// <summary>
		/// Computes the view matrix based on the cameras state, which is used to transform
		/// vertices from world space to camera space.
		/// </summary>
		/// <returns></returns>
		glm::mat4 generate_view_matrix() const;

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

		/// <summary>
		/// When the observed Input issues a command, transform the camera in accordance
		/// </summary>
		/// <param name="command"></param>
		void process_input_command(KbdInputCommand command);

		/// <summary>
		/// Rotate the camera depending on mouse pos
		/// </summary>
		void process_mouse_movement(float x_offset, float y_offst);

		/// <summary>
		/// The scene that has registered this Camera
		/// </summary>
		World* _world;

		float yaw = -90.f;
		float pitch = 0.0f;

		float lastX = 800.0f / 2.0f;
		float lastY = 600.0f / 2.0f;
		bool firstMouse = true;

	private:
		/// <summary>
		/// Recompute the forward and up vectors in response to rotation
		/// </summary>
		void update_camera_vectors();
	};

}

