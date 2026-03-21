#include "pch.h"
#include "Billboard.h"
#include "Constants.h"
#include "Camera.h"
#include "Vertex.h"
#include "Mesh.h"
#include "MeshGenerator.h"

bool Billboard::Init(HWND hWnd) 
{
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
	assert(SUCCEEDED(hr), "CreateBuffer() failed: constant buffer per object");

	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(PerFrame);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&constBufferDesc, nullptr, &perFrameBuffer);
	assert(SUCCEEDED(hr), "CreateBuffer() failed: constant buffer per frame");

	camera = std::make_unique<Camera>(hWnd);

	Vertex vertex;
	vertex.pos = glm::vec3(0.0f, 0.0f, 0.0f);

	auto [vertices, indices, topology] = MeshGenerator::GeneratePoint();
	auto point = std::make_shared<Mesh>(device.Get(), vertices, indices, topology);

	auto shader = std::make_shared<Shader>(device.Get(), L"BillboardVS.hlsl", L"", L"", L"BillboardGS.hlsl", L"BillboardPS.hlsl");

	auto materialData = std::make_shared<Material::Data>();
	materialData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	materialData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	materialData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	materialData->shininess = 32.0f;

	auto material = std::make_shared<Material>(device.Get(), materialData, shader, "tree.png");

	for (int i = 0; i < 20; i++) {
		auto tree1 = std::make_unique<Object>(point, material);
		auto tree2 = std::make_unique<Object>(point, material);

		auto scale = glm::vec3(2.0f, 2.0f, 2.0f);
		tree1->SetScale(scale);
		tree2->SetScale(scale);

		auto translation1 = glm::vec3(-2.5f, 0.0f, -10.0f + (float)i * 2.0f);
		auto translation2 = glm::vec3(2.5f, 0.0f, -10.0f + (float)i * 2.0f);
		tree1->SetTranslation(translation1);
		tree2->SetTranslation(translation2);

		trees.push_back(std::move(tree1));
		trees.push_back(std::move(tree2));
	}

	return true;
}

void Billboard::Update()
{
	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
		camera->SetPitch(0.0f);
	}

	ImGui::Begin("Billboard");

	ImGui::End();

	// per frame
	const auto view = camera->GetView();
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	PerFrame perFrame;
	perFrame.viewProj = glm::transpose(proj * view);
	perFrame.eyePos = camera->GetPos();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer.Get(), 0);
}

void Billboard::Render() 
{
	//const float clearColor[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), clearColor);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	ID3D11Buffer *constantBuffers[] = { perObjectBuffer.Get(), perFrameBuffer.Get()};
	context->VSSetConstantBuffers(0, 2, constantBuffers);
	context->GSSetConstantBuffers(0, 2, constantBuffers);
	context->PSSetConstantBuffers(0, 2, constantBuffers);

	for (auto &object : trees) {
		object->Render(context.Get(), perObjectBuffer.Get());
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
