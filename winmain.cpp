#include <Windows.h>
#include <d2d1.h>
#include <math.h>
#pragma comment(lib,"d2d1.lib")

const wchar_t gClassName[]{ L"WindowClass" };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

ID2D1Factory* gpD2DFactroy{};
ID2D1HwndRenderTarget* gpRenderTarget{};
ID2D1SolidColorBrush* gpBrush{};
ID2D1RadialGradientBrush* gpRadialBrush{};

void OnPaint(HWND hwnd);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevinstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactroy);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"FAiled to create Factory", L"Error", MB_OK);
		return 0;
	}


	WNDCLASSEX wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to Window Class", L"Error", MB_OK);
		return 0;
	}

	HWND hwnd;
	RECT wr = { 0,0,1024,768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, NULL);
	hwnd = CreateWindowEx(
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

	if (hwnd == nullptr)
	{
		MessageBox(NULL, L"Failed to create window class!", L"Error", MB_OK);	
		return 0;
	}

	GetClientRect(hwnd, &wr);
	hr = gpD2DFactroy->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, 
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
		&gpRenderTarget
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create D2D RenderTarget", L"Error", MB_OK);
		return 0;
	}

	hr = gpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &gpBrush);

	ID2D1GradientStopCollection* pGradinetStops{};
	D2D1_GRADIENT_STOP gradientStops[2];

	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1);
	gradientStops[1].position = 1.0f;

	hr = gpRenderTarget->CreateGradientStopCollection(gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradinetStops
	);

	if (SUCCEEDED(hr))
	{
		hr = gpRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(50, 150),
				D2D1::Point2F(0, 0),
				50,
				50),
			pGradinetStops,
			&gpRadialBrush
		);
	}

	if (pGradinetStops != nullptr)
	{
		pGradinetStops->Release();
		pGradinetStops = nullptr;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
	MSG msg;
	
	while (true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
			OnPaint(hwnd);
		}
	}
	

	if (gpRadialBrush != nullptr)
	{
		gpRadialBrush->Release();
		gpRadialBrush = nullptr;
	}

	if (gpBrush != nullptr)
	{
		gpBrush->Release();
		gpBrush = nullptr;
	}

	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	if (gpD2DFactroy != nullptr)
	{
		gpD2DFactroy->Release();
		gpD2DFactroy = nullptr;
	}


	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	gpBrush->SetOpacity(1.0f);
	gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));
	gpRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 100.f, 100.f), gpBrush);
	
	gpBrush->SetOpacity(0.5f);
	gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
	gpRenderTarget->FillRectangle(D2D1::RectF(50.0f, 50.0f, 150.f, 150.f), gpRadialBrush);

	static float fAngle = 0.0f;

	gpBrush->SetOpacity(2.0f);
	gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
	gpRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(75.f+sinf(fAngle)*25, 150.0f), 50.0f, 50.0f), gpRadialBrush);

	fAngle += 0.2f;

	gpRenderTarget->EndDraw();
	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		break;

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