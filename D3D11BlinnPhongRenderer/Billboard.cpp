#include "pch.h"
#include "Billboard.h"
#include "Camera.h"
#include "Vertex.h"

bool Billboard::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	camera = new Camera(hWnd);
	camera->width = width;
	camera->height = height;

	Vertex vertex;
	vertex.pos = glm::vec3(0.0f, 0.0f, 0.0f);

	// rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	// vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = &vertex;
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: vertex buffer" << std::endl;
		return false;
	}

	// constant buffer
	D3D11_BUFFER_DESC constBufferDesc = { 0 };
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(PerObject);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&constBufferDesc, nullptr, &perObjectBuffer);
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

	// vertex shader
	ID3DBlob *vsBlob = nullptr;
	hr = D3DCompileFromFile(L"BillboardVS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: vertex shader" << std::endl;
		return false;
	}

	// input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(inputElementDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);
	vsBlob->Release();

	// geometry shader
	ID3DBlob *gsBlob = nullptr;
	hr = D3DCompileFromFile(L"BillboardGS.hlsl", nullptr, nullptr, "main", "gs_5_0", 0, 0, &gsBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: geometry shader" << std::endl;
		return false;
	}

	device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), nullptr, &gs);
	gsBlob->Release();

	// pixel shader
	ID3DBlob *psBlob = nullptr;
	hr = D3DCompileFromFile(L"BillboardPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();

	return true;
}

void Billboard::Update() {
	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
	}

	// per object
	const auto view = camera->view;
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	PerObject perObject;
	perObject.viewProj = glm::transpose(proj * view);

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perObject, sizeof(PerObject));
	context->Unmap(perObjectBuffer, 0);

	// per frame
	PerFrame perFrame;
	perFrame.eyePos = camera->pos;

	context->Map(perFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer, 0);

	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Billboard");

	ImGui::End();
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
	context->IASetInputLayout(inputLayout);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer, perFrameBuffer };
	context->GSSetConstantBuffers(0, 2, constant_buffers);
	context->VSSetShader(vs, nullptr, 0);
	context->GSSetShader(gs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);
	context->PSSetShaderResources(0, 1, &srv);
	context->PSSetSamplers(0, 1, &samplerState);

	context->Draw(1, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
