#pragma once
#include <memory>
struct GLFWwindow;

namespace whim {

	// Forward declarations
	class Camera;
	class Input;
	class World
	{
	public:
		World();

		/// <summary>
		/// Determines which Camera this world should use
		/// </summary>
		/// <param name="camera"></param>
		void register_camera(Camera* camera);

		/// <summary>
		/// Determines which Input this world should use
		/// </summary>
		/// <param name="camera"></param>
		void register_input(std::unique_ptr<Input> input);

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
		/// The active camera rendering out the World
		/// In the future, we may allow for this camera to be swapped out
		/// </summary>
		Camera* worldCamera;
		/// <summary>
		/// We can only have one Input active at a time per world
		/// We will respond to events received from this input
		/// </summary>
		std::unique_ptr<Input> input;
		GLFWwindow* window;

		/// <summary>
		/// These constants define the area of a chunk
		/// 
		/// We will generate blocks for the chunk within the volume defined by the constants below
		/// </summary>
		static const unsigned int CHUNK_SIZE_WIDTH = 16; // Size of a chunk along the x axis
		static const unsigned int CHUNK_SIZE_DEPTH = 16; // Size of a chunk along the z axis
		static const unsigned int CHUNK_SIZE_HEIGHT = 16; // Size of a chunk along y axis

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


