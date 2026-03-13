#include "pch.h"
#include "BlinnPhong.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshGenerator.h"
#include "TextureLoader.h"

bool BlinnPhong::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

	camera = new Camera(hWnd);
	camera->width = width;
	camera->height = height;

	light = new Light();

	mesh = new Mesh();
	//MeshGenerator::GenerateCube(*mesh);
	MeshGenerator::GenerateSphere(*mesh);

	material = new Material();
	material->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	material->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material->shininess = 32.0f;

	// vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = mesh->vertices.size() * sizeof(Vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA verticesData = { 0 };
	verticesData.pSysMem = mesh->vertices.data();
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &verticesData, &vertexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: vertex buffer" << std::endl;
		return false;
	}

	// index buffer
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = mesh->indices.size() * sizeof(int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indicesData = { 0 };
	indicesData.pSysMem = mesh->indices.data();
	hr = device->CreateBuffer(&indexBufferDesc, &indicesData, &indexBuffer);
	if (FAILED(hr)) {
		std::cout << "CreateBuffer() failed: index buffer" << std::endl;
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
	hr = D3DCompileFromFile(L"BlinnPhongVS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
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
	hr = D3DCompileFromFile(L"BlinnPhongPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) {
		std::cout << "D3DCompileFromFile() failed: pixel shader" << std::endl;
		return false;
	}

	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();

	// sampler
	D3D11_SAMPLER_DESC samplerDesc = { 0 };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr)) {
		std::cout << "CreateSamplerState() failed." << std::endl;
		return false;
	}

	// texture
	TextureLoader::CreateTexture(device, "./brick-texture.png", &srv);

	return true;
}

void BlinnPhong::Update() {
	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
	}

	ImGui::Begin("Blinn-Phong");

	ImGui::Text("material");

	if (ImGui::SliderFloat("ambient", &material->ambient.x, 0.0f, 1.0f)) {
		material->ambient.y = material->ambient.x;
		material->ambient.z = material->ambient.x;
	}
	if (ImGui::SliderFloat("diffuse", &material->diffuse.x, 0.0f, 1.0f)) {
		material->diffuse.y = material->diffuse.x;
		material->diffuse.z = material->diffuse.x;
	}
	if (ImGui::SliderFloat("specular", &material->specular.x, 0.0f, 1.0f)) {
		material->specular.y = material->specular.x;
		material->specular.z = material->specular.x;
	}
	ImGui::SliderFloat("shininess", &material->shininess, 1.0f, 256.0f);
	ImGui::Checkbox("use texture", &useTexture);

	ImGui::Separator();

	ImGui::Text("light");
	if (ImGui::RadioButton("directional", light->type == LightType::Directional)) {
		light->type = LightType::Directional;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("point", light->type == LightType::Point)) {
		light->type = LightType::Point;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("spot", light->type == LightType::Spot)) {
		light->type = LightType::Spot;
	}
	static auto rotation = glm::vec3(45.0f, 0.0f, 0.0f);
	if (light->type == LightType::Directional) {
		if (ImGui::DragFloat3("rotation", &rotation.x, 1.0f)) {
			const float yaw = glm::radians(rotation.y);
			const float pitch = glm::radians(rotation.x);

			light->dir.x = cos(pitch) * sin(yaw);
			light->dir.y = -sin(pitch);
			light->dir.z = cos(pitch) * cos(yaw);
		}
	} else if (light->type == LightType::Point) {
		ImGui::DragFloat3("position", &light->pos.x, 0.1f);
		ImGui::SliderFloat("fall off start", &light->fallOffStart, 0.0f, 10.0f);
		ImGui::SliderFloat("fall off end", &light->fallOffEnd, 0.0f, 50.0f);
	} else if (light->type == LightType::Spot) {
		ImGui::DragFloat3("position", &light->pos.x, 0.1f);
		if (ImGui::DragFloat3("rotation", &rotation.x, 1.0f)) {
			const float yaw = glm::radians(rotation.y);
			const float pitch = glm::radians(rotation.x);

			light->dir.x = cos(pitch) * sin(yaw);
			light->dir.y = -sin(pitch);
			light->dir.z = cos(pitch) * cos(yaw);
		}
		ImGui::SliderFloat("fall off start", &light->fallOffStart, 0.0f, 10.0f);
		ImGui::SliderFloat("fall off end", &light->fallOffEnd, 0.0f, 50.0f);
		ImGui::SliderFloat("spot power", &light->spotPower, 1.0f, 128.0f);
	}
	ImGui::SliderFloat("strength", &light->strength, 0.0f, 5.0f);

	ImGui::End();

	// per object
	const auto model = glm::mat4(1.0f);
	const auto view = camera->view;
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	PerObject perObject;

	perObject.model = glm::transpose(model);
	perObject.viewProj = glm::transpose(proj * view);
	perObject.modelInvTr = glm::transpose(glm::inverseTranspose(model));

	perObject.material.ambient = material->ambient;
	perObject.material.diffuse = material->diffuse;
	perObject.material.specular = material->specular;
	perObject.material.shininess = material->shininess;

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perObject, sizeof(PerObject));
	context->Unmap(perObjectBuffer, 0);

	// per frame
	PerFrame perFrame;
	perFrame.light = *light;
	perFrame.eyePos = camera->pos;
	perFrame.useTexture = useTexture;

	context->Map(perFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer, 0);

	
}

void BlinnPhong::Render() {
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
	context->PSSetShaderResources(0, 1, &srv);
	context->PSSetSamplers(0, 1, &samplerState);

	context->DrawIndexed(mesh->indices.size(), 0, 0);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
