#include "Scene.h"


using namespace whim;

Scene::Scene()
{
}

void Scene::register_camera(Camera* camera)
{
	this->sceneCamera = camera;
	this->sceneCamera->_scene = this;
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
	input->process_input();
}