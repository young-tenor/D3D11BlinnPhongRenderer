#pragma once
#include "App.h"
#include <wrl.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

class BlinnPhong : public App {
public:
	~BlinnPhong() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct PerFrame {
		glm::mat4 viewProj;
		Light light;
		glm::vec3 eyePos;
		int useTexture;
	};

	ComPtr<ID3D11Buffer> perObjectBuffer;
	ComPtr<ID3D11Buffer> perFrameBuffer;

	bool useTexture = false;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	std::unique_ptr<Object> object;
	std::shared_ptr<Material::Data> materialData;
};

