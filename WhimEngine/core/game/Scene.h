#pragma once
#include "../rendering/Camera.hpp"
#include "../input/Input.hpp"

namespace whim {

	class Scene
	{
	public:
		Scene();

		/// <summary>
		/// Determines which Camera this schene should use
		/// </summary>
		/// <param name="camera"></param>
		/// <returns>Returns pointer to the scene this method was called on</returns>
		Scene* register_camera(Camera* camera);

		/// <summary>
		/// Determines which Input this schene should use
		/// </summary>
		/// <param name="camera"></param>
		/// <returns>Returns pointer to the scene this method was called on</returns>
		Scene* register_input(Input* input);

		/// <summary>
		/// Determines the GLFWwindow this scene should be rendered to
		/// </summary>
		/// <param name="window"></param>
		/// <returns></returns>
		Scene* register_window(GLFWwindow* window);

		/// <summary>
		/// The main function which processes events for the scene
		/// </summary>
		void tick();

		
	private:
		Camera* sceneCamera;
		Input* input;
		GLFWwindow* window;
		double lastTime = glfwGetTime();


		/// <summary>
		/// Returns delta time for scene
		/// </summary>
		/// <returns></returns>
		double getDeltaTime() const {
			double deltaTime = glfwGetTime() - lastTime;
			return deltaTime;
		}


	};

}


