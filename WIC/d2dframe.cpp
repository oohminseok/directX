#include <sstream>
#include "d2dframe.h"

#pragma comment(lib,"d2d1.lib")

HRESULT d2dframe::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc{ };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to Register Window Class!", L"Error", MB_OK);
		return 0;
	}


	RECT wr{ 0,0,static_cast<LONG>(width),static_cast<LONG>(height)};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);


	mHwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Direct2D",
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

	if (mHwnd == NULL)
	{
		MessageBox(NULL, L"Failed to Create Window!", L"Error", MB_OK);
		return 0;
	}

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	return S_OK;
}

HRESULT d2dframe::InitD2D(HWND hwnd)
{
	HRESULT hr;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.GetAddressOf()
	);

	ThrowIfFailed(hr);

	RECT wr{};

	GetClientRect(hwnd, &wr);
	hr = mspD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
		mspRenderTarget.GetAddressOf()
	);

	ThrowIfFailed(hr);

	return S_OK;
}

void d2dframe::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	InitWindow(hInstance, title, width, height);
	InitD2D(mHwnd);

	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);
}

void d2dframe::Release()
{
	mspRenderTarget.Reset();
	mspD2DFactory.Reset();
}

void d2dframe::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspRenderTarget->EndDraw();
}

int d2dframe::GameLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.wParam == WM_QUIT)
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

void d2dframe::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
	std::wostringstream oss;
	oss << "Error code:" << error << std::endl << msg;


	MessageBox(NULL, oss.str().c_str(), title, MB_OK);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	d2dframe* pFramework = reinterpret_cast<d2dframe*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

	}
	return 0;
}