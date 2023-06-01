#include "bitmap.h"

void bitmap::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance,title,width,height);

	mspBackBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);

	HRESULT hr = gpRenderTarget->CreateBitmap(D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
		mspFrameBitamp.GetAddressOf());

	ThrowIfFailed(hr);
}

void bitmap::Render()
{
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspBackBuffer[0] = 255;
	mspBackBuffer[1] = 0;
	mspBackBuffer[2] = 0;
	mspBackBuffer[3] = 255;

	mspBackBuffer[4] = 255;
	mspBackBuffer[5] = 0;
	mspBackBuffer[6] = 0;
	mspBackBuffer[7] = 255;

	PresentBuffer();
	gpRenderTarget->DrawBitmap(mspFrameBitamp.Get());

	gpRenderTarget->EndDraw();
}

void bitmap::PresentBuffer()
{
	mspFrameBitamp->CopyFromMemory(nullptr, &mspBackBuffer[0],
		BITMAP_WIDTH * BITMAP_BYTECOUNT);
}
