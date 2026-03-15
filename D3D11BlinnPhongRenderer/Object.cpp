#include "pch.h"
#include "Object.h"
#include "Constants.h"

Object::Object(Mesh &mesh, Material &material) : mesh(&mesh), material(&material) {
}

void Object::Render(ID3D11DeviceContext &context, ID3D11Buffer *perObjectBuffer, const glm::mat4 &viewProj) {
	PerObject perObject;
	perObject.model = glm::transpose(glm::mat4(1.0f));
	perObject.viewProj = glm::transpose(viewProj);
	perObject.modelInvTr = glm::transpose(glm::inverseTranspose(perObject.model));
	perObject.material = material->GetData();

	D3D11_MAPPED_SUBRESOURCE resource;
	context.Map(perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perObject, sizeof(PerObject));
	context.Unmap(perObjectBuffer, 0);

	mesh->Bind(context);
	material->Bind(context);
	context.DrawIndexed(mesh->GetIndexCount(), 0, 0);
}
