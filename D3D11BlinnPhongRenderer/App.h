#pragma once

class App {
public:
	virtual ~App() = default;

	virtual bool Init(HWND hWnd);
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	HWND hWnd = nullptr;
	float width = 1280.0f;
	float height = 720.0f;
	float aspect = 1280.0f / 720.0f;

	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *context = nullptr;
	IDXGISwapChain *swapChain = nullptr;

	D3D11_VIEWPORT viewport = { 0 };
	ID3D11RenderTargetView *rtv = nullptr;
	ID3D11DepthStencilView *dsv = nullptr;
};