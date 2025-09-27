#pragma once
#include<glm/glm.hpp>

struct Vertex
{
	glm::vec3 position; // 3D position
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};
