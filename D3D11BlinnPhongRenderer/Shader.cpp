#include "pch.h"
#include "Shader.h"

Shader::Shader(ID3D11Device &device) {
	CreateVertexShaderAndInputLayout(device);
	CreatePixelShader(device);
}

void Shader::Bind(ID3D11DeviceContext &context) const {
	context.IASetInputLayout(inputLayout);
	context.VSSetShader(vs, nullptr, 0);
	context.PSSetShader(ps, nullptr, 0);
}

void Shader::CreateVertexShaderAndInputLayout(ID3D11Device &device) {
	ID3DBlob *vsBlob = nullptr;
	HRESULT	hr = D3DCompileFromFile(L"BlinnPhongVS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: vertex shader");
	device.CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device.CreateInputLayout(inputElementDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	vsBlob->Release();
	assert(SUCCEEDED(hr), "CreateInputLayout() failed.");
}

void Shader::CreatePixelShader(ID3D11Device &device) {
	ID3DBlob *psBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(L"BlinnPhongPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: pixel shader");
	device.CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();
}
