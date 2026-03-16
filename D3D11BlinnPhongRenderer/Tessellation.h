#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Object.h"

class Tessellation : public App {
public:
	~Tessellation() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct PerFrame {
		Light light;
		glm::vec3 eyePos;
		float padding;
	};

	ID3D11RasterizerState *wireframeRasterizerState = nullptr;

	ID3D11Buffer *perObjectBuffer = nullptr;
	ID3D11Buffer *perFrameBuffer = nullptr;

	//ID3D11SamplerState *samplerState = nullptr;
	//ID3D11ShaderResourceView *srv = nullptr;

	bool drawWireFrame = false;

	Camera *camera = nullptr;
	Light *light = nullptr;
	Object *object = nullptr;
};

