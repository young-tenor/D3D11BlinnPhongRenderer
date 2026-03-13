#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include "Camera.h"

class Billboard : public App {
public:
	~Billboard() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct PerObject {
		glm::mat4 viewProj;
	};

	struct PerFrame {
		glm::vec3 eyePos;
		float padding;
	};

	ID3D11InputLayout *inputLayout = nullptr;
	ID3D11VertexShader *vs = nullptr;
	ID3D11GeometryShader *gs = nullptr;
	ID3D11PixelShader *ps = nullptr;

	ID3D11Buffer *vertexBuffer = nullptr;
	ID3D11Buffer *perObjectBuffer = nullptr;
	ID3D11Buffer *perFrameBuffer = nullptr;

	ID3D11SamplerState *samplerState = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;

	Camera *camera = nullptr;
};

