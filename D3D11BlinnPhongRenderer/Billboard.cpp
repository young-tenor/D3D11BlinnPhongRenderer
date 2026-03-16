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

	camera = std::make_unique<Camera>(hWnd);

	Vertex vertex;
	vertex.pos = glm::vec3(0.0f, 0.0f, 0.0f);

	// tree
	{
		auto [vertices, indices] = MeshGenerator::GeneratePoint();
		auto point = std::make_shared<Mesh>(device.Get(), vertices, indices);

		auto shader = std::make_shared<Shader>(device.Get(), L"BillboardVS.hlsl", L"BillboardGS.hlsl", L"BillboardPS.hlsl");

		auto materialData = std::make_shared<Material::Data>();
		materialData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		materialData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
		materialData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		materialData->shininess = 32.0f;

		auto material = std::make_shared<Material>(device.Get(), materialData, shader, "tree.png");

		tree = std::make_unique<Object>(point, material);
	}

	// floor
	{
		auto [vertices, indices] = MeshGenerator::GenerateSquare();
		auto square = std::make_shared<Mesh>(device.Get(), vertices, indices);

		auto shader = std::make_shared<Shader>(device.Get(), L"BlinnPhongVS.hlsl", L"", L"BlinnPhongPS.hlsl");

		auto materialData = std::make_shared<Material::Data>();
		materialData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		materialData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
		materialData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		materialData->shininess = 32.0f;

		auto material = std::make_shared<Material>(device.Get(), materialData, shader, "brick-texture.png");

		floor = std::make_unique<Object>(square, material);

		auto scale = glm::vec3(10.0f, 10.0f, 10.0f);
		auto rotation = glm::vec3(90.0f, 0.0f, 0.0f);
		auto translation = glm::vec3(0.0f, -0.5f, 0.0f);
		floor->SetScale(scale);
		floor->SetRotation(rotation);
		floor->SetTranslation(translation);
	}

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
	perFrame.eyePos = camera->GetPos();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer.Get(), 0);
}

void Billboard::Render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	//const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), clear_color);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer.Get(), perFrameBuffer.Get()};
	context->VSSetConstantBuffers(0, 2, constant_buffers);
	context->GSSetConstantBuffers(0, 2, constant_buffers);
	context->PSSetConstantBuffers(0, 2, constant_buffers);

	const auto view = camera->GetView();
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	tree->Render(context.Get(), perObjectBuffer.Get(), proj * view);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	floor->Render(context.Get(), perObjectBuffer.Get(), proj * view);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
