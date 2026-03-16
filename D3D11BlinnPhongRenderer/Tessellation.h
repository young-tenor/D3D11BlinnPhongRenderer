#pragma once
#include "App.h"
#include <wrl.h>
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

	ComPtr<ID3D11RasterizerState> wireframeRasterizerState;

	ComPtr<ID3D11Buffer> perObjectBuffer;
	ComPtr<ID3D11Buffer> perFrameBuffer;

	//ID3D11SamplerState *samplerState = nullptr;
	//ID3D11ShaderResourceView *srv = nullptr;

	bool drawWireFrame = false;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	std::unique_ptr<Object> object;
};

