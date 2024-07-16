#include "Scene.h"


using namespace whim;

Scene::Scene(Camera* sceneCamera)
{
    this->sceneCamera = sceneCamera;
}

Camera* Scene::getCamera()
{
    return sceneCamera;
}
