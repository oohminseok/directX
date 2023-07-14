#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>

class D3DFramework
{
private:
	const std::wstring gClassName{ L"MyWindowClass" };
	const std::wstring gTitle{ L"Direct3D Sample" };

protected:
	int mWindowWidth{ 800 };
	int mWindowHeight{ 600 };
	HWND mHwnd{};
	HINSTANCE mInstance{};
	bool mMinimized{ false };
	bool mMaximized{ false };
	bool gResizing{ false };

	Microsoft::WRL::ComPtr<ID3D11Device> mspDevice{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mspDeviceContext{};
	Microsoft::WRL::ComPtr<IDXGISwapChain> mspSwapChanin{};
	//Resource
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mspRenderTarget{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mspDepthStencil{};
	//Resource-View
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mspRenderTargetView{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mspDepthStecilView{};

private:
	void InitWinodw(HINSTANCE hInstance);
	void InitD3D();

protected:
	void OnResize();
	void DestroyD3D();
	void RenderFrame();
	virtual void Render() {};

public:
	virtual void Initizlize(HINSTANCE hInstance,
		int widht=800,
		int height=600
	);
	virtual void Destory();
	virtual void GameLoop();

public:
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

};

