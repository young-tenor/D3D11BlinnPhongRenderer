#include "pch.h"
#include "Material.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Material::Material(
	ID3D11Device *device,
	std::shared_ptr<Data> data,
	std::shared_ptr<Shader> shader,
	const std::string &texturePath
) 
	: shader(std::move(shader)) 
{
	if (data == nullptr) {
		this->data = std::make_shared<Data>();

		this->data->ambient = glm::vec3(0.1f);
		this->data->diffuse = glm::vec3(0.7f);
		this->data->specular = glm::vec3(0.5f);
		this->data->shininess = 32.0f;
	} else {
		this->data = std::move(data);
	}

	if (!texturePath.empty()) {
		CreateTexture(device, texturePath);
	}

	CreateSamplerState(device);
}

Material::Material(
	ID3D11Device *device,
	std::shared_ptr<Data> data,
	std::shared_ptr<Shader> shader,
	const std::vector<UINT> &image,
	const UINT width, const UINT height) :
	Material(device, data, shader, ""
	) 
{
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = (UINT)width;
	textureDesc.Height = (UINT)height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initialData = { };
	initialData.pSysMem = image.data();
	initialData.SysMemPitch = width * sizeof(UINT);

	ID3D11Texture2D *texture2D = nullptr;
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &texture2D);
	assert(SUCCEEDED(hr), "CreateTexture2D() failed.");

	hr = device->CreateShaderResourceView(texture2D, nullptr, &srv);
	texture2D->Release();
	assert(SUCCEEDED(hr), "CreateShaderResourceView() failed.");
}

void Material::Bind(ID3D11DeviceContext *context) const
{
	if (shader) {
		shader->Bind(context);
	}
	if (srv) {
		context->PSSetShaderResources(0, 1, srv.GetAddressOf());
	}
	if (samplerState) {
		context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	}
}

void Material::CreateTexture(ID3D11Device *device, const std::string &fileName)
{
	int width, height, channels;
	unsigned char *imageData = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	assert(imageData, "stbi_load() failed.");

	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = (UINT)width;
	textureDesc.Height = (UINT)height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initialData = { };
	initialData.pSysMem = imageData;
	initialData.SysMemPitch = (UINT)(width * 4);

	ID3D11Texture2D *texture2D = nullptr;
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &texture2D);
	stbi_image_free(imageData);
	assert(SUCCEEDED(hr), "CreateTexture2D() failed.");

	hr = device->CreateShaderResourceView(texture2D, nullptr, &srv);
	texture2D->Release();
	assert(SUCCEEDED(hr), "CreateShaderResourceView() failed.");
}

void Material::CreateSamplerState(ID3D11Device *device)
{
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	assert(SUCCEEDED(hr), "CreateSamplerState() failed.");
}
