#pragma once
#include "App.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

class Billboard : public App {
public:
	~Billboard() override = default;

	bool Init(HWND hWnd) override;
	void Update() override;
	void Render() override;

private:
	struct Material {
		glm::vec3 ambient;
		float shininess;
		glm::vec3 diffuse;
		float padding1;
		glm::vec3 specular;
		float padding2;
	};

	struct PerObject {
		glm::mat4 model;
		glm::mat4 viewProj;
		glm::mat4 modelInvTr;

		Material material;
	};

	struct PerFrame {
		Light light;
		glm::vec3 eyePos;
		int useTexture;
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
	Mesh *floor = nullptr;
};

