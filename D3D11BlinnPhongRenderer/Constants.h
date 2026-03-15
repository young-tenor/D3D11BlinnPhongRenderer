#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "Light.h"

struct PerObject {
	glm::mat4 model;
	glm::mat4 viewProj;
	glm::mat4 modelInvTr;

	Material::Data material;
};

struct PerFrame {
	Light light;
	glm::vec3 eyePos;
	int useTexture;
};
