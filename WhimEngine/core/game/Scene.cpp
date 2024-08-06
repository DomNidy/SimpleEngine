#include "Scene.h"


using namespace whim;

Scene::Scene()
{
}

Scene* Scene::register_camera(Camera* camera)
{
	this->sceneCamera = camera;
	return this;
}

Scene* Scene::register_window(GLFWwindow* window) {
	this->window = window;
	return this;
}


Scene* Scene::register_input(Input* input)
{
	if (this->window == nullptr) {
		throw std::runtime_error("You must register a window for a scene before attempting to register an Input! (call register_window)");
	}


	this->input = input;
	
	// Register key callback to call the process_inputs method of our input system
	// We cant pass pointers to non-static member functions because the signatures dont match
	// member functions have an implicit this pointer
	// so their signature expands to:
	// void (Scene::*)(int x, int y)
	// non member functions signatures expand to
	// void (*)(int x, int y)
	// This is why we experience an error, because glfwSetKeyCallback is receiving incorrect signature

	
	glfwSetKeyCallback(this->window, input->process_input);
	return this;
}
