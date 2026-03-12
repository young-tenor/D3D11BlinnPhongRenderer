#pragma once
#include <glm/glm.hpp>

enum class LightType {
	Directional,
	Point,
	Spot,
};

struct Light {
	glm::vec3 pos = glm::vec3(0.0f, 3.0f, -3.0f);
	float strength = 1.0f;
	glm::vec3 dir = glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f));
	float fallOffStart = 1.0f;
	float fallOffEnd = 10.0f;
	float spotPower = 64.0f;
	LightType type = LightType::Directional;
	float padding;
};
