#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Material.h"

class Object {
public:
	Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	void Render(ID3D11DeviceContext *context, ID3D11Buffer *perObjectBuffer, const glm::mat4 &viewProj) const;

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
};
