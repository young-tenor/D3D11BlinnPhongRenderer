#pragma once
#include "App.h"
#include "Camera.h"
#include "Object.h"

class Mipmap : public App
{
public:
	~Mipmap() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct PerFrame {
		glm::mat4 viewProj;
		glm::vec3 eyePos;
		int useMipmap;
	};

	ComPtr<ID3D11Buffer> perObjectBuffer;
	ComPtr<ID3D11Buffer> perFrameBuffer;

	bool useMipmap = false;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Object> floor;
};
