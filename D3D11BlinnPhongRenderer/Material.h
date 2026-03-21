#pragma once
#include <wrl.h>
#include <glm/glm.hpp>
#include "Shader.h"

using Microsoft::WRL::ComPtr;

class Material
{
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
	Material(
		ID3D11Device *device, 
		std::shared_ptr<Data> data, 
		std::shared_ptr<Shader> shader, 
		const std::string &texturePath
	);
	Material(
		ID3D11Device *device,
		std::shared_ptr<Data> data,
		std::shared_ptr<Shader> shader, 
		const std::vector<UINT> &texture,
		const UINT width,
		const UINT height
	);

	const Data &GetData() const { return *data; }

	void Bind(ID3D11DeviceContext *context) const;

private:
	void CreateTexture(ID3D11Device *device, const std::string &fileName);
	void CreateSamplerState(ID3D11Device *device);

private:
	std::shared_ptr<Data> data;
	std::shared_ptr<Shader> shader;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11SamplerState> samplerState;
};