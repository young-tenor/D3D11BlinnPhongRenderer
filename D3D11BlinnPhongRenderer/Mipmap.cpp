	#include "pch.h"
	#include "Mipmap.h"
	#include "Constants.h"
	#include "MeshGenerator.h"

	bool Mipmap::Init(HWND hWnd)
	{
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
		assert(SUCCEEDED(hr), "CreateBuffer() failed: constant buffer per object");

		constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufferDesc.ByteWidth = sizeof(PerFrame);
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = device->CreateBuffer(&constBufferDesc, nullptr, &perFrameBuffer);
		assert(SUCCEEDED(hr), "CreateBuffer() failed: constant buffer per frame");

		camera = std::make_unique<Camera>(hWnd);

		auto [vertices, indices, topology] = MeshGenerator::GenerateSquare();
		auto mesh = std::make_shared<Mesh>(device.Get(), vertices, indices, topology);

		auto shader = std::make_shared<Shader>(device.Get(), L"MipmapVS.hlsl", L"", L"", L"", L"MipmapPS.hlsl");

		const UINT width = 1024;
		const UINT height = 1024;
		const UINT cellSize = 32;

		std::vector<UINT> checker(width * height);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				bool isWhite = ((j / cellSize) + (i / cellSize)) % 2 == 0;
				checker[i * width + j] = isWhite ? 0xFFFFFFFF : 0xFF000000;
			}
		}

		auto material = std::make_shared<Material>(device.Get(), nullptr, shader, checker, width, height);

		floor = std::make_unique<Object>(mesh, material);

		floor->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
		floor->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		floor->SetTranslation(glm::vec3(0.0f, -1.0f, 0.0f));

		return true;
	}

	void Mipmap::Update()
	{
		// GUI
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Mipmap");

		ImGui::Checkbox("use mipmap", &useMipmap);

		ImGui::End();

		// per frame
		const auto view = camera->GetView();
		const auto proj = glm::perspectiveLH_ZO(glm::radians(45.0f), aspect, 0.1f, 100.0f);

		PerFrame perFrame;
		perFrame.viewProj = glm::transpose(proj * view);
		perFrame.eyePos = camera->GetPos();
		perFrame.useMipmap = useMipmap;

		D3D11_MAPPED_SUBRESOURCE resource;
		context->Map(perFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &perFrame, sizeof(PerFrame));
		context->Unmap(perFrameBuffer.Get(), 0);
	}

	void Mipmap::Render()
	{
		const float clear_color[] = { 0.1f, 0.2f, 0.4f, 1.0f };
		//const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		context->ClearRenderTargetView(rtv.Get(), clear_color);
		context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		context->RSSetViewports(1, &viewport);
		context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

		ID3D11Buffer *constant_buffers[] = { perObjectBuffer.Get(), perFrameBuffer.Get() };
		context->VSSetConstantBuffers(0, 2, constant_buffers);
		context->PSSetConstantBuffers(0, 2, constant_buffers);

		floor->Render(context.Get(), perObjectBuffer.Get());

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swapChain->Present(1, 0);
	}
