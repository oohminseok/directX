#pragma once
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <map>
#include <string>
#include "exception.h"

class bitmapmanager
{
public:
	static bitmapmanager& Instance()
	{
		static bitmapmanager instance;
		return instance;
	}

private:
	bitmapmanager() = default;
	bitmapmanager(const bitmapmanager&) = delete;
	void operator=(const bitmapmanager&) = delete;

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};

	std::map <std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources;

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();

	ID2D1Bitmap* LoadBitmap(std::wstring filename);

};

