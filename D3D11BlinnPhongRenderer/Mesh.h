#pragma once
#include <vector>
#include "Vertex.h"

class Mesh {
public:
public:
	Mesh(ID3D11Device &device, const std::vector<Vertex> &vertices, const std::vector<UINT> &indices);

	const UINT GetIndexCount() const { return indices.size(); }

	void Bind(ID3D11DeviceContext &context) const;

private:
	void CreateVertexBuffer(ID3D11Device &device);
	void CreateIndexBuffer(ID3D11Device &device);

private:
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	ID3D11Buffer *vertexBuffer = nullptr;
	ID3D11Buffer *indexBuffer = nullptr;
};
