#include "pch.h"
#include "Tessellation.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshGenerator.h"

bool Tessellation::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	camera = new Camera(hWnd);
	camera->width = width;
	camera->height = height;

	light = new Light();

	mesh = new Mesh();
	MeshGenerator::GenerateSphere(*mesh, 0.5f, 16, 16);

	material = new Material();
	material->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	material->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material->shininess = 32.0f;

	D3D11_RASTERIZER_DESC resterizerDesc = { };
	resterizerDesc.CullMode = D3D11_CULL_NONE;
	resterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&resterizerDesc, &wireframeRasterizerState);

	// vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = mesh->vertices.size() * sizeof(Vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA verticesData = { };
	verticesData.pSysMem = mesh->vertices.data();
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &verticesData, &vertexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: vertex buffer" << std::endl;
		return false;
	}

	// index buffer
	D3D11_BUFFER_DESC indexBufferDesc = { };
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = mesh->indices.size() * sizeof(int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indicesData = { };
	indicesData.pSysMem = mesh->indices.data();
	hr = device->CreateBuffer(&indexBufferDesc, &indicesData, &indexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: index buffer" << std::endl;
		return false;
	}

	// constant buffer
	D3D11_BUFFER_DESC constBufferDesc = { };
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
	hr = D3DCompileFromFile(L"TessellationVS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
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

	// pixel shader
	ID3DBlob *psBlob = nullptr;
	hr = D3DCompileFromFile(L"TessellationPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();

	// sampler
	//D3D11_SAMPLER_DESC samplerDesc = { };
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//samplerDesc.MinLOD = 0;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	//if (FAILED(hr)) {
	//	std::cout << "CreateSamplerState() failed." << std::endl;
	//	return false;
	//}

	// texture
	//TextureLoader::CreateTexture(device, "./brick-texture.png", &srv);

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

	// per object
	const auto model = glm::mat4(1.0f);
	const auto view = camera->view;
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	PerObject perObject;
	perObject.viewProj = glm::transpose(proj * view);
	perObject.material = *material;

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perObject, sizeof(PerObject));
	context->Unmap(perObjectBuffer, 0);

	// per frame
	PerFrame perFrame;
	perFrame.light = *light;
	perFrame.eyePos = camera->pos;

	context->Map(perFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer, 0);
}

void Tessellation::Render() {
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
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer, perFrameBuffer };
	context->VSSetConstantBuffers(0, 1, &perObjectBuffer);
	context->PSSetConstantBuffers(0, 2, constant_buffers);
	context->VSSetShader(vs, nullptr, 0);
	context->PSSetShader(ps, nullptr, 0);
	//context->PSSetShaderResources(0, 1, &srv);
	//context->PSSetSamplers(0, 1, &samplerState);

	if (drawWireFrame) {
		context->RSSetState(wireframeRasterizerState);
	} else {
		context->RSSetState(nullptr);
	}

	context->DrawIndexed(mesh->indices.size(), 0, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
