#include "pch.h"
#include "BlinnPhong.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshGenerator.h"

bool BlinnPhong::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	camera = new Camera(hWnd);
	camera->width = width;
	camera->height = height;

	cube = new Mesh();
	MeshGenerator::GenerateCube(*cube);

	// vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = cube->vertices.size() * sizeof(Vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA verticesData = { 0 };
	verticesData.pSysMem = cube->vertices.data();
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &verticesData, &vertexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: vertex buffer" << std::endl;
		return false;
	}

	// index buffer
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = cube->indices.size() * sizeof(int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indicesData = { 0 };
	indicesData.pSysMem = cube->indices.data();
	hr = device->CreateBuffer(&indexBufferDesc, &indicesData, &indexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: index buffer" << std::endl;
		return false;
	}

	// constant buffer
	D3D11_BUFFER_DESC constBufferDesc = { 0 };
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(MVP);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&constBufferDesc, nullptr, &constantBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: constant buffer" << std::endl;
		return false;
	}

	// vertex shader
	ID3DBlob *vsBlob = nullptr;
	hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: vertex shader" << std::endl;
		return false;
	}

	// input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(inputElementDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);
	vsBlob->Release();

	// pixel shader
	ID3DBlob *psBlob = nullptr;
	hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();

	return true;
}

void BlinnPhong::Update() {
	camera->Update();

	MVP mvp;
	mvp.model = glm::mat4(1.0f);
	mvp.view = camera->view;
	mvp.projection = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	mvp.model = glm::transpose(mvp.model);
	mvp.view = glm::transpose(mvp.view);
	mvp.projection = glm::transpose(mvp.projection);

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &mvp, sizeof(MVP));
	context->Unmap(constantBuffer, 0);
}

void BlinnPhong::Render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(rtv, clear_color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, dsv);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetInputLayout(inputLayout);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetConstantBuffers(0, 1, &constantBuffer);
	context->VSSetShader(vs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);

	context->DrawIndexed(cube->indices.size(), 0, 0);

	swapChain->Present(1, 0);
}
