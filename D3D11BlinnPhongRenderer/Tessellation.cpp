#include "pch.h"
#include "Tessellation.h"
#include "Constants.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshGenerator.h"

bool Tessellation::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	D3D11_RASTERIZER_DESC resterizerDesc = { };
	resterizerDesc.CullMode = D3D11_CULL_NONE;
	resterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&resterizerDesc, &wireframeRasterizerState);

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

	light = std::make_unique<Light>();

	auto [vertices, indices, topology] = MeshGenerator::GenerateSphere(1.0f, 16, 16);
	auto mesh = std::make_shared<Mesh>(device.Get(), vertices, indices, topology);

	auto shader = std::make_shared<Shader>(device.Get(), L"TessellationVS.hlsl", L"", L"TessellationPS.hlsl");

	auto materialData = std::make_shared<Material::Data>();
	materialData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	materialData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	materialData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	materialData->shininess = 32.0f;
	auto material = std::make_shared<Material>(device.Get(), materialData, shader, "");

	object = std::make_unique<Object>(mesh, material);

	return true;
}

void Tessellation::Update() {
	static auto rotation = glm::vec3(45.0f, 0.0f, 0.0f);

	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
	}

	ImGui::Begin("Tessellation");

	ImGui::Checkbox("draw wireframe", &drawWireFrame);

	ImGui::End();

	// per frame
	PerFrame perFrame;
	perFrame.light = *light;
	perFrame.eyePos = camera->GetPos();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer.Get(), 0);
}

void Tessellation::Render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	//const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), clear_color);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer.Get(), perFrameBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, &constant_buffers[0]);
	context->PSSetConstantBuffers(0, 2, constant_buffers);

	if (drawWireFrame) {
		context->RSSetState(wireframeRasterizerState.Get());
	} else {
		context->RSSetState(nullptr);
	}

	const auto view = camera->GetView();
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	object->Render(context.Get(), perObjectBuffer.Get(), proj * view);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
