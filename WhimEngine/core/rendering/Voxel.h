#pragma once
#pragma once

#include "Vertex.h"
#include <vector>

class Voxel
{
public:
	Voxel(int xpos, int ypos, int zpos);

	void translate(glm::vec3 vec);
	std::vector<whim::Vertex> verts;

private:
};

