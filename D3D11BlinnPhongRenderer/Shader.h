#pragma once

class Shader {
public:
	Shader(ID3D11Device &device);
	
	void Bind(ID3D11DeviceContext &context) const;

private:
	void CreateVertexShaderAndInputLayout(ID3D11Device &device);
	void CreatePixelShader(ID3D11Device &device);

private:
	ID3D11InputLayout *inputLayout = nullptr;
	ID3D11VertexShader *vs = nullptr;
	ID3D11PixelShader *ps = nullptr;
};

