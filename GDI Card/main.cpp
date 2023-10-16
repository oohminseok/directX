#include "GameLogic.h"

#pragma comment(lib,"Gdiplus.lib")

const wchar_t gClassName[] = L"myWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

solitaire::GameLogic gLogic;

int WINAPI WinMain(
	_In_ HINSTANCE hinstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);


	WNDCLASSEX wc{};
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpszClassName = gClassName;
	wc.hIcon = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_3DLIGHT;
	wc.hInstance = hinstance;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to windowclass", L"FAIL", MB_OK);
		return 0;
	}

	HWND hwnd;
	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024,
		768,
		NULL,
		NULL,
		hinstance,
		NULL
	);

	if (hwnd == nullptr)
	{
		MessageBox(NULL, L"Failed to createWindow", L"Fail", MB_OK);
		return 0;
	}

	gLogic.Init(hwnd);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);

}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	Gdiplus::Graphics graphcis(hdc);

	gLogic.Draw(graphcis);

	EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		break;

	case WM_LBUTTONUP:
		gLogic.OnClick(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	{
		std::wostringstream oss;
		oss << "virtual" << wParam
			<< ",Extra=" << lParam;
		OutputDebugString(oss.str().c_str());
		break;
	}

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
