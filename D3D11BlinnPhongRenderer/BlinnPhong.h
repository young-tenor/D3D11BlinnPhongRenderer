#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Material.h"

class BlinnPhong : public App {
public:
	~BlinnPhong() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	ID3D11Buffer *perObjectBuffer = nullptr;
	ID3D11Buffer *perFrameBuffer = nullptr;

	bool useTexture = false;

	Camera *camera = nullptr;
	Light *light = nullptr;
	Object *object = nullptr;
	Material::Data *materialData = nullptr;
};

