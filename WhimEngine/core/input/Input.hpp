#pragma once
#include "../rendering/Camera.hpp"
#include <unordered_map>
#include <string>
#include "../logger/Logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputCommand.hpp"

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
		Scene* _scene;

		friend class Scene;
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
	};
}

