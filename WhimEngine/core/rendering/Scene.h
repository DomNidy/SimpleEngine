#pragma once
#include "Camera.hpp"

namespace whim {

	class Scene
	{
	public:
		Scene(Camera* sceneCamera);
		Camera* getCamera();

	private:
		Camera* sceneCamera;
	};

}


