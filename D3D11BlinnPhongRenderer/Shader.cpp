#include "pch.h"
#include "Shader.h"

Shader::Shader(ID3D11Device &device, const std::wstring &vsPath, const std::wstring &gsPath, const std::wstring &psPath) {
	CreateVertexShaderAndInputLayout(device, vsPath);
	if (!gsPath.empty()) {
		CreateGeometryShader(device, gsPath);
	}
	CreatePixelShader(device, psPath);
}

void Shader::Bind(ID3D11DeviceContext &context) const {
	context.IASetInputLayout(inputLayout);
	context.VSSetShader(vs, nullptr, 0);
	context.GSSetShader(gs, nullptr, 0);
	context.PSSetShader(ps, nullptr, 0);
}

void Shader::CreateGeometryShader(ID3D11Device &device, const std::wstring &gsPath) {
	ID3DBlob *blob = nullptr;
	HRESULT hr = D3DCompileFromFile(gsPath.c_str(), nullptr, nullptr, "main", "gs_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: geometry shader");
	device.CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &gs);
	blob->Release();
}

void Shader::CreateVertexShaderAndInputLayout(ID3D11Device &device, const std::wstring &vsPath) {
	ID3DBlob *vsBlob = nullptr;
	HRESULT	hr = D3DCompileFromFile(vsPath.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
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

void Shader::CreatePixelShader(ID3D11Device &device, const std::wstring &psPath) {
	ID3DBlob *psBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(psPath.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: pixel shader");
	device.CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	psBlob->Release();
}
