#pragma once

struct GLFWwindow;

namespace whim {

	// Forward declarations
	class Camera;
	class Input;
	class Scene
	{
	public:
		Scene();

		/// <summary>
		/// Determines which Camera this schene should use
		/// </summary>
		/// <param name="camera"></param>
		void register_camera(Camera* camera);

		/// <summary>
		/// Determines which Input this schene should use
		/// </summary>
		/// <param name="camera"></param>
		void register_input(Input* input);

		/// <summary>
		/// The main function which processes events for the scene
		/// </summary>
		void tick();

		/// <summary>
		/// Returns delta time for scene
		/// </summary>
		/// <returns></returns>
		double get_delta_time() const;
		
		const Camera* get_camera() const;
		const Input* get_input() const;
	private:
		/// <summary>
		/// The active camera rendering out the Scene
		/// In the future, we may allow for this camera to be swapped out
		/// </summary>
		Camera* sceneCamera;
		/// <summary>
		/// We can only have one Input active at a time per scene
		/// We will respond to events received from this input
		/// </summary>
		Input* input;
		GLFWwindow* window;

		/// <summary>
		/// Returns the time the last frame rendered at
		/// </summary>
		double last_time;

		/// <summary>
		/// The time since last frame
		/// </summary>
		double delta_time;

		void update_delta_time();
	};

}


