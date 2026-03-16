#pragma once
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class App {
public:
	virtual ~App() = default;

	virtual bool Init(HWND hWnd);
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	HWND hWnd = nullptr;
	UINT width = 1280;
	UINT height = 720;
	float aspect = 1280.0f / 720.0f;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;

	D3D11_VIEWPORT viewport = { };
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsv;
};