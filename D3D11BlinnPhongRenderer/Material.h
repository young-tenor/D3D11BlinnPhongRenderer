#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Material {
public:
	struct Data {
		glm::vec3 ambient;
		float shininess;
		glm::vec3 diffuse;
		float padding1;
		glm::vec3 specular;
		float padding2;
	};

public:
	Material(ID3D11Device &device, const Data &data, const Shader &shader, const std::string &texturePath);

	const Data &GetData() const { return *data; }

	void Bind(ID3D11DeviceContext &context) const;

private:
	void CreateTexture(ID3D11Device &device, const std::string &fileName);
	void CreateSamplerState(ID3D11Device &device);

private:
	const Data *data = nullptr;
	const Shader *shader = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;
	ID3D11SamplerState *samplerState = nullptr;
};