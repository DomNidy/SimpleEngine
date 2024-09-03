#pragma once
#include "../rendering/Camera.hpp"
#include <unordered_map>
#include <string>
#include "../logger/Logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputCommand.hpp"

struct MousePosition {
	float x;
	float y;
};

namespace whim {

	class Input
	{
	public:
		Input(GLFWwindow* window);
		Input();

		/// <summary>
		/// Register a Camera as an observer of this Input
		/// </summary>
		void register_observer(Camera* camera);

		/// <summary>
		/// Unregister a Camera as an observer of this Input
		/// </summary>
		void unregister_observer(Camera* camera);
		/// <summary>
		/// The scene that has registered this Camera
		/// </summary>
		World* _scene;

		friend class World;
	protected:
		void process_input();
		/// <summary>
		/// The GLFWwindow we will read input from
		/// </summary>
		GLFWwindow* window;
	private:
		std::unordered_map<KbdInputCommand, int> kbd_input_command_map;
		/// <summary>
		/// All Camera's that observe this Input will respond to the input which this Input producers
		/// </summary>
		std::vector<Camera*> observers;

		/// <summary>
		/// Notify all cameras observing this Input, that an input event occurred
		/// </summary>
		/// <param name="command"></param>
		void notify_observers(KbdInputCommand command);

		/// <summary>
		/// Notify all cameras observing this Input, that a mouse movement event occured
		/// </summary>
		/// <param name="command"></param>
		void notify_observers(float x_offset, float y_offset);

		/// <summary>
		/// Update mouse position
		/// </summary>
		void process_input_mouse();

		MousePosition current_pos = {640.f, 350.0f};
		MousePosition delta_pos = { 0.0f, 0.0f };
	};
}

