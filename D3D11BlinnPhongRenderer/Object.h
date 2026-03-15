#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Material.h"

class Object {
public:
	Object(Mesh &mesh, Material &material);

	void Render(ID3D11DeviceContext &context, ID3D11Buffer *perObjectBuffer, const glm::mat4 &viewProj);

private:
	Mesh *mesh;
	Material *material;
};
