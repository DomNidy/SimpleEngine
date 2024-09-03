#include "World.h"
#include "../logger/Logger.hpp"
#include "../rendering/Camera.hpp"
#include "../input/Input.hpp"
using namespace whim;

World::World()
{
	delta_time = 0.00001f;
	last_time = glfwGetTime();
}

void World::register_camera(Camera* camera)
{
	this->worldCamera = camera;
	this->worldCamera->_world = this;
}



double whim::World::get_delta_time() const
{
	return delta_time;
}

const Camera* World::get_camera() const
{
	return this->worldCamera;
};

const Input* World::get_input() const {
	return this->input;
}

void whim::World::update_delta_time()
{
	double now= glfwGetTime();
	delta_time = now - last_time;
	last_time = now;
}

void World::register_input(Input* input)
{
	if (input->window == nullptr) {
		Logger::log_warning("You registered an input, but the input you registered has no associated window, so no input will ever be received");
	}

	this->input = input;
	this->input->_scene = this;
}

void World::tick() {
	update_delta_time();
	input->process_input();
}