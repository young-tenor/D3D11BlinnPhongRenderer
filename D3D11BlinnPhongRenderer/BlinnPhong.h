#pragma once
#include "App.h"
#include "Camera.h"
#include "Mesh.h"

struct MVP {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

class BlinnPhong : public App {
public:
	~BlinnPhong() override = default;

	bool Init(HWND h_wnd) override;
	void Update() override;
	void Render() override;

private:
	ID3D11InputLayout *inputLayout = nullptr;
	ID3D11VertexShader *vs = nullptr;
	ID3D11PixelShader *ps = nullptr;

	ID3D11Buffer *vertexBuffer = nullptr;
	ID3D11Buffer *indexBuffer = nullptr;
	ID3D11Buffer *constantBuffer = nullptr;

	ID3D11SamplerState *samplerState = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;

	Camera *camera = nullptr;
	Mesh *mesh = nullptr;
};

