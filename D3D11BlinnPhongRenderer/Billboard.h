#pragma once
#include "App.h"
#include <wrl.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Object.h"

using Microsoft::WRL::ComPtr;


class Billboard : public App {
public:
	~Billboard() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct PerFrame {
		Light light;
		glm::vec3 eyePos;
		float padding;
	};

	ComPtr<ID3D11Buffer> perObjectBuffer;
	ComPtr<ID3D11Buffer> perFrameBuffer;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Object> object;
	//std::unique_ptr<Object> floor;
};

