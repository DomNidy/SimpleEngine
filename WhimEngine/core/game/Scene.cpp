#include "Scene.h"
#include "../logger/Logger.hpp"
#include "../rendering/Camera.hpp"
#include "../input/Input.hpp"
using namespace whim;

Scene::Scene()
{
	delta_time = 0.00001f;
	last_time = glfwGetTime();
}

void Scene::register_camera(Camera* camera)
{
	this->sceneCamera = camera;
	this->sceneCamera->_scene = this;
}



double whim::Scene::get_delta_time() const
{
	return delta_time;
}

const Camera* Scene::get_camera() const
{
	return this->sceneCamera;
};

const Input* Scene::get_input() const {
	return this->input;
}

void whim::Scene::update_delta_time()
{
	double now= glfwGetTime();
	delta_time = now - last_time;
	last_time = now;
}

void Scene::register_input(Input* input)
{
	if (input->window == nullptr) {
		Logger::log_warning("You registered an input, but the input you registered has no associated window, so no input will ever be received");
	}

	this->input = input;
	this->input->_scene = this;
}

void Scene::tick() {
	update_delta_time();
	input->process_input();
}