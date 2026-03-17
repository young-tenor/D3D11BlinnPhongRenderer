#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"

Mesh::Mesh(ID3D11Device *device, const std::vector<Vertex> &vertices, const std::vector<UINT> &indices, D3D11_PRIMITIVE_TOPOLOGY topology) :
	vertices(vertices), 
	indices(indices),
	topology(topology) {
	CreateVertexBuffer(device);
	CreateIndexBuffer(device);
}

void Mesh::Bind(ID3D11DeviceContext *context) const {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(topology);
}

void Mesh::CreateVertexBuffer(ID3D11Device *device) {
	D3D11_BUFFER_DESC desc = { };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = vertices.size() * sizeof(Vertex);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { };
	data.pSysMem = vertices.data();
	HRESULT hr = device->CreateBuffer(&desc, &data, &vertexBuffer);

	assert(SUCCEEDED(hr), "CreateBuffer() failed: vertex buffer");
}

void Mesh::CreateIndexBuffer(ID3D11Device *device) {
	D3D11_BUFFER_DESC desc = { };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = indices.size() * sizeof(UINT);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { };
	data.pSysMem = indices.data();
	HRESULT hr = device->CreateBuffer(&desc, &data, &indexBuffer);

	assert(SUCCEEDED(hr), "CreateBuffer() failed: index buffer");
}
