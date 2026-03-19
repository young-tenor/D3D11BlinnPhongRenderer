#include "pch.h"
#include "Shader.h"

Shader::Shader(
	ID3D11Device *device, 
	const std::wstring &vsPath,
	const std::wstring &hsPath,
	const std::wstring &dsPath,
	const std::wstring &gsPath, 
	const std::wstring &psPath) {
	CreateVertexShaderAndInputLayout(device, vsPath);
	if (!hsPath.empty()) {
		CreateHullShader(device, hsPath);
	}
	if (!dsPath.empty()) {
		CreateDomainShader(device, dsPath);
	}
	if (!gsPath.empty()) {
		CreateGeometryShader(device, gsPath);
	}
	CreatePixelShader(device, psPath);
}

void Shader::Bind(ID3D11DeviceContext *context) const {
	context->IASetInputLayout(inputLayout.Get());
	context->VSSetShader(vs.Get(), nullptr, 0);
	context->HSSetShader(hs.Get(), nullptr, 0);
	context->DSSetShader(ds.Get(), nullptr, 0);
	context->GSSetShader(gs.Get(), nullptr, 0);
	context->PSSetShader(ps.Get(), nullptr, 0);
}

void Shader::CreateVertexShaderAndInputLayout(ID3D11Device *device, const std::wstring &vsPath) {
	ID3DBlob *blob = nullptr;
	HRESULT	hr = D3DCompileFromFile(vsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: vertex shader");
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputElementDesc, 3, blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();
	assert(SUCCEEDED(hr), "CreateInputLayout() failed.");
}

void Shader::CreateHullShader(ID3D11Device *device, const std::wstring &hsPath) {
	ID3DBlob *blob = nullptr;
	HRESULT hr = D3DCompileFromFile(hsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "hs_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: hull shader");
	device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &hs);
	blob->Release();
}

void Shader::CreateDomainShader(ID3D11Device *device, const std::wstring &dsPath) {
	ID3DBlob *blob = nullptr;
	HRESULT hr = D3DCompileFromFile(dsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ds_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: domain shader");
	device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ds);
	blob->Release();
}

void Shader::CreateGeometryShader(ID3D11Device *device, const std::wstring &gsPath) {
	ID3DBlob *blob = nullptr;
	HRESULT hr = D3DCompileFromFile(gsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: geometry shader");
	device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &gs);
	blob->Release();
}

void Shader::CreatePixelShader(ID3D11Device *device, const std::wstring &psPath) {
	ID3DBlob *blob = nullptr;
	HRESULT hr = D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &blob, nullptr);
	assert(SUCCEEDED(hr), "D3DCompileFromFile() failed: pixel shader");
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps);
	blob->Release();
}
