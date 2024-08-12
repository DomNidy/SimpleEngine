#include "Voxel.h"

static const std::vector <whim::Vertex>  base_verts = {
	// Front face ( red )
	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, // bottom right
	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, // bottom left
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.f), glm::vec2(0.0f, 1.0f)}, // top right
	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.f, 0.0f, 0.f), glm::vec2(1.0f, 1.0f)}, // top left

	// Back face ( green )
	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.5f)},
	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f)},

	// Left face ( blue )
	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 0.0f)},
	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.0f, 0.5f)},
	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.5f, 0.5f)},

	// Right face (yellow)
	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.9f, 0.9f, 0.f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.9f, 0.9f, 0.f), glm::vec2(0.5f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.9f, 0.9f, 0.f), glm::vec2(0.0f, 0.5f)},
	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.9f, 0.9f, 0.f), glm::vec2(0.5f, 0.5f)},

	// Top face ( purple )
	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(.5f, 0.0f, .5f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(.5f, 0.0f, .5f), glm::vec2(0.5f, 0.0f)},
	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(.5f, 0.0f, .5f), glm::vec2(0.0f, 0.5f)},
	{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(.5f, 0.0f, .5f), glm::vec2(0.5f, 0.5f)},

	// Bottom face ( orange )
	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.5f, 0.f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.5f, 0.f), glm::vec2(0.5f, 0.0f)},
	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.f, 0.5f, 0.f), glm::vec2(0.0f, 0.5f)},
	{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.f, 0.5f, 0.f), glm::vec2(0.5f, 0.5f)},
};

Voxel::Voxel(int xpos, int ypos, int zpos)
{
	verts = base_verts;
	translate(glm::vec3(xpos, ypos, zpos));
}

void Voxel::translate(glm::vec3 vec) {
	for (int i = 0; i < verts.size(); i++) {
		verts[i].position += vec;
	}
}
