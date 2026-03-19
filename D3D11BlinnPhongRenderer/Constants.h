#pragma once
#include <glm/glm.hpp>
#include "Material.h"

struct PerObject {
	glm::mat4 model;
	glm::mat4 modelInvTr;

	Material::Data material;
};
