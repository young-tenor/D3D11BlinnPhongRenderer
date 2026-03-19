#include "pch.h"
#include "BlinnPhong.h"
#include "MeshGenerator.h"
#include "Shader.h"
#include "Constants.h"

bool BlinnPhong::Init(HWND hWnd) {
	if (!App::Init(hWnd)) {
		return false;
	}

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

	auto [vertices, indices, topology] = MeshGenerator::GenerateSphere();
	auto mesh = std::make_shared<Mesh>(device.Get(), vertices, indices, topology);

	auto shader = std::make_shared<Shader>(device.Get(), L"BlinnPhongVS.hlsl", L"", L"", L"", L"BlinnPhongPS.hlsl");

	materialData = std::make_shared<Material::Data>();
	materialData->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	materialData->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	materialData->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	materialData->shininess = 32.0f;
	auto material = std::make_shared<Material>(device.Get(), materialData, shader, "brick-texture.png");

	object = std::make_unique<Object>(mesh, material);

	return true;
}

void BlinnPhong::Update() {
	static auto rotation = glm::vec3(45.0f, 0.0f, 0.0f);

	// GUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::GetIO().WantCaptureMouse) {
		camera->Update();
	}

	ImGui::Begin("Blinn-Phong");

	ImGui::Text("material");

	if (ImGui::SliderFloat("ambient", &materialData->ambient.x, 0.0f, 1.0f)) {
		materialData->ambient.y = materialData->ambient.x;
		materialData->ambient.z = materialData->ambient.x;
	}
	if (ImGui::SliderFloat("diffuse", &materialData->diffuse.x, 0.0f, 1.0f)) {
		materialData->diffuse.y = materialData->diffuse.x;
		materialData->diffuse.z = materialData->diffuse.x;
	}
	if (ImGui::SliderFloat("specular", &materialData->specular.x, 0.0f, 1.0f)) {
		materialData->specular.y = materialData->specular.x;
		materialData->specular.z = materialData->specular.x;
	}
	ImGui::SliderFloat("shininess", &materialData->shininess, 1.0f, 256.0f);
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

	// per frame
	const auto view = camera->GetView();
	const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	PerFrame perFrame;
	perFrame.viewProj = glm::transpose(proj * view);
	perFrame.light = *light;
	perFrame.eyePos = camera->GetPos();
	perFrame.useTexture = useTexture;

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(perFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &perFrame, sizeof(PerFrame));
	context->Unmap(perFrameBuffer.Get(), 0);
}

void BlinnPhong::Render() {
	const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
	//const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), clear_color);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	ID3D11Buffer *constant_buffers[] = { perObjectBuffer.Get(), perFrameBuffer.Get() };
	context->VSSetConstantBuffers(0, 2, constant_buffers);
	context->PSSetConstantBuffers(0, 2, constant_buffers);
	
	object->Render(context.Get(), perObjectBuffer.Get());

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0);
}
