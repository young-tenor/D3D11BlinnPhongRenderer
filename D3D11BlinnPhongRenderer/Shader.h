#pragma once
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class Shader {
public:
	Shader(
		ID3D11Device *device, 
		const std::wstring &vsPath,
		const std::wstring &hsPath,
		const std::wstring &dsPath,
		const std::wstring &gsPath, 
		const std::wstring &psPath);
	
	void Bind(ID3D11DeviceContext *context) const;

private:
	void CreateVertexShaderAndInputLayout(ID3D11Device *device, const std::wstring &vsPath);
	void CreateHullShader(ID3D11Device *device, const std::wstring &hsPath);
	void CreateDomainShader(ID3D11Device *device, const std::wstring &dsPath);
	void CreateGeometryShader(ID3D11Device *device, const std::wstring &gsPath);
	void CreatePixelShader(ID3D11Device *device, const std::wstring &psPath);

private:
	ComPtr<ID3D11InputLayout> inputLayout;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11HullShader> hs;
	ComPtr<ID3D11DomainShader> ds;
	ComPtr<ID3D11GeometryShader> gs;
	ComPtr<ID3D11PixelShader> ps;
};

