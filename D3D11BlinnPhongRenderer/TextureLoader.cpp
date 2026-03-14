#include "pch.h"
#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool TextureLoader::CreateTexture(ID3D11Device *device, const char *fileName, ID3D11ShaderResourceView **srv) {
    int width, height, channels;
    unsigned char *imageData = stbi_load(fileName, &width, &height, &channels, STBI_rgb_alpha);
    if (!imageData) {
        std::cout << "stbi_load() failed." << std::endl;
        return false;
    }

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
    if (FAILED(hr)) {
        std::cout << "CreateTexture2D() failed." << std::endl;
        return false;
    }


    hr = device->CreateShaderResourceView(texture2D, nullptr, srv);
    texture2D->Release();
    if (FAILED(hr)) {
        std::cout << "CreateShaderResourceView() failed." << std::endl;
        return false;
    }

    return true;
}
