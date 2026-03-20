#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Material.h"

class Object {
public:
	Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	void SetScale(const glm::vec3 &scale) { this->scale = scale; }
	void SetRotation(const glm::vec3 &rotation) { this->rotation = rotation; }
	void SetTranslation(const glm::vec3 &translation) { this->translation = translation; }

	void Render(ID3D11DeviceContext *context, ID3D11Buffer *perObjectBuffer) const;

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
};
