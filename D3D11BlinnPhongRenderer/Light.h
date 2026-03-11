#pragma once
#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 pos = glm::vec3(0.0f, 3.0f, -3.0f);
	float strength = 1.0f;
};

