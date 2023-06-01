#pragma once
#include "D2DFramework.h"

const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 };

class bitmap : public D2DFramework
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitamp;
	std::unique_ptr<UINT8[]> mspBackBuffer;

public:
	virtual void Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width = 1024, UINT height = 768) override;
	void Render() override;

	void PresentBuffer();
};

