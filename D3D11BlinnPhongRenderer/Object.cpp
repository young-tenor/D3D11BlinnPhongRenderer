#include "pch.h"
#include "Object.h"
#include "Constants.h"

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
	mesh(std::move(mesh)), 
	material(std::move(material)) {
}

void Object::Render(ID3D11DeviceContext *context, ID3D11Buffer *perObjectBuffer) const {
	const auto s = glm::scale(glm::mat4(1.0f), scale);
	auto r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	r = glm::rotate(r, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	r = glm::rotate(r, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	const auto t = glm::translate(glm::mat4(1.0f), translation);
	const auto model = t * r * s;

	PerObject perObject;
	perObject.model = glm::transpose(model);
	perObject.modelInvTr = glm::transpose(glm::inverseTranspose(model));
	perObject.material = material->GetData();


	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perObject, sizeof(PerObject));
	context->Unmap(perObjectBuffer, 0);

	mesh->Bind(context);
	material->Bind(context);
	context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}
