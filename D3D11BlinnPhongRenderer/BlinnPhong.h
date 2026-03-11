#pragma once
#include "App.h"
#include "Camera.h"
#include "Mesh.h"

struct PerObject {
	glm::mat4 mvp;
	glm::mat4 modelInvTr;
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

