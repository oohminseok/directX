#include <sstream>
#include "D2DFramework.h"

#pragma comment(lib,"d2d1.lib")

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to Window Class", L"Error", MB_OK);
		return 0;
	}

	HWND hwnd;
	RECT wr = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height)};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, NULL);
	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == nullptr)
	{
		MessageBox(NULL, L"Failed to create window class!", L"Error", MB_OK);
		return 0;
	}

	return S_OK;
}

HRESULT D2DFramework::InitD2D(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, gpD2DFactroy.GetAddressOf());

	ThrowIfFailed(hr);
	
	

	RECT wr{};
	GetClientRect(hwnd, &wr);
	hr = gpD2DFactroy->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
		gpRenderTarget.GetAddressOf()
	);

	ThrowIfFailed(hr);

	return S_OK;

}

void D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	InitWindow(hInstance, title, width, height);
	InitD2D(mHwnd);

	ShowWindow(mHwnd,SW_SHOW);
	UpdateWindow(mHwnd);
}

void D2DFramework::Release()
{
	gpRenderTarget.Reset();
	gpD2DFactroy.Reset();
}

void D2DFramework::Render()
{
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));


	gpRenderTarget->EndDraw();
}

int D2DFramework::GameLoop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Render();
		}
	}
	
	return static_cast<int>(msg.wParam);
}

void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
	std::wostringstream oss;

	oss << "Error Code:" << error << std::endl << msg;

	MessageBox(NULL, oss.str().c_str(), title, MB_OK);

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
