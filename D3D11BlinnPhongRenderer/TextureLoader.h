#pragma once

class TextureLoader {
public:
	static bool CreateTexture(ID3D11Device *device, const char *fileName, ID3D11ShaderResourceView **srv);
};

