#include "pch.h"
#include "App.h"

bool App::Init(HWND hWnd) 
{
	this->hWnd = hWnd;

	RECT rc;
	GetClientRect(hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	aspect = (float)width / height;

	// device
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels, 
		1,
		D3D11_SDK_VERSION, 
		&device,
		nullptr, 
		&context
	);
	if (FAILED(hr)) {
		std::cout << "D3D11CreateDevice() failed." << std::endl;
		return false;
	}

	// swap chain
	UINT msaaQuality = 0;
	device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (msaaQuality > 0) {
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = msaaQuality - 1;
	} else {
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	device.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	ComPtr<IDXGIFactory> dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	hr = dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
	if (FAILED(hr)) {
		std::cout << "CreateSwapChain() failed." << std::endl;
		return false;
	}

	// viewport
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	// render target view
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		std::cout << "GetBuffer() failed." << std::endl;
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtv);
	if (FAILED(hr)) {
		std::cout << "CreateRenderTargetView() failed." << std::endl;
		return false;
	}

	// depth stencil view
	ComPtr<ID3D11Texture2D> depthBuffer;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { };
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc = swapChainDesc.SampleDesc;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
	device->CreateDepthStencilView(depthBuffer.Get(), nullptr, &dsv);

	// GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), context.Get());

	return true;
}