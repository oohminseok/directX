#pragma once

#include <wincodec.h>
#include "d2dframe.h"
class imageExample :
    public d2dframe
{
    Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;
   
public:
    virtual void Initialize(HINSTANCE hInstance, LPCWSTR title = L"BMP File Example", UINT width = 1024, UINT height = 768)override;
    void Render()override;
    void Release()override;

public:
    HRESULT LoadBMP(LPCWSTR filename);
    HRESULT LoadWICImage(LPCWSTR filename);
};

