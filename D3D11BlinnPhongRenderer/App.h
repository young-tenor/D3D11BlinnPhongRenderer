#pragma once

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

	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *context = nullptr;
	IDXGISwapChain *swapChain = nullptr;

	D3D11_VIEWPORT viewport = { };
	ID3D11RenderTargetView *rtv = nullptr;
	ID3D11DepthStencilView *dsv = nullptr;
};