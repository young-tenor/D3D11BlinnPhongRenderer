#pragma once
#include <vector>
#include <wrl.h>
#include "Vertex.h"

using Microsoft::WRL::ComPtr;

class Mesh 
{
public:
	Mesh(
		ID3D11Device *device,
		const std::vector<Vertex> &vertices, 
		const std::vector<UINT> &indices, 
		D3D11_PRIMITIVE_TOPOLOGY topology
	);

	const UINT GetIndexCount() const { return indices.size(); }

	void Bind(ID3D11DeviceContext *context) const;

private:
	void CreateVertexBuffer(ID3D11Device *device);
	void CreateIndexBuffer(ID3D11Device *device);

private:
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	D3D11_PRIMITIVE_TOPOLOGY topology;
};
