#include "pch.h"
#include "Billboard.h"
#include "Constants.h"
#include "Camera.h"
#include "Vertex.h"
#include "Mesh.h"
#include "MeshGenerator.h"

bool Billboard::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	// rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	// constant buffer
	D3D11_BUFFER_DESC constBufferDesc = { };
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(PerObject);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->CreateBuffer(&constBufferDesc, nullptr, &perObjectBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: constant buffer per object" << std::endl;
		return false;
	}

	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(PerFrame);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&constBufferDesc, nullptr, &perFrameBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: constant buffer per frame" << std::endl;
		return false;
	}

	camera = new Camera(hWnd);
	camera->width = width;
	camera->height = height;

	Vertex vertex;
	vertex.pos = glm::vec3(0.0f, 0.0f, 0.0f);

	auto [vertices, indices] = MeshGenerator::GeneratePoint();
	auto point = new Mesh(*device, vertices, indices);

	auto shader = new Shader(*device, L"BillboardVS.hlsl", L"BillboardGS.hlsl", L"BillboardPS.hlsl");

	auto treeMatData = new Material::Data();
	treeMatData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	treeMatData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	treeMatData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	treeMatData->shininess = 32.0f;
	auto treeMat = new Material(*device, *treeMatData, *shader, "tree.png");

	object = new Object(*point, *treeMat);

	//floor = new Mesh();
	//MeshGenerator::GenerateSquare(*floor);

	return true;
}

void Billboard::Update() {
	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
	}

	ImGui::Begin("Billboard");

	ImGui::End();

	// per frame
	PerFrame perFrame;
	perFrame.eyePos = camera->pos;

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer, 0);
}

void Billboard::Render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	//const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv, clear_color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, dsv);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer, perFrameBuffer };
	context->GSSetConstantBuffers(0, 2, constant_buffers);

	const auto view = camera->view;
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	object->Render(*context, perObjectBuffer, proj * view);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
