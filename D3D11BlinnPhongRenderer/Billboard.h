#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Object.h"

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

	ID3D11Buffer *perObjectBuffer = nullptr;
	ID3D11Buffer *perFrameBuffer = nullptr;

	Camera *camera = nullptr;
	Object *object = nullptr;
	Object *floor = nullptr;
};

