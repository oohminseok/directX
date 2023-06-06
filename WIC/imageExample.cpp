#include <fstream>
#include <vector>
#include "imageExample.h"

#pragma comment(lib,"WindowsCodecs.lib")

void imageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    CoInitialize(nullptr);

    HRESULT hr=CoCreateInstance(CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(mspWICFactory.GetAddressOf())
        );
    ThrowIfFailed(hr);

    d2dframe::Initialize(hInstance, title, width, height);

    LoadWICImage(L"Data/bug.png");

}

void imageExample::Render()
{
    mspRenderTarget->BeginDraw();

    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));
    mspRenderTarget->DrawBitmap(mspBitmap.Get(),D2D1::RectF(10,10,100,50));

    mspRenderTarget->EndDraw();


}

void imageExample::Release()
{
    mspBitmap.Reset();
    d2dframe::Release();

    mspWICFactory.Reset();
    CoUninitialize();
}

HRESULT imageExample::LoadBMP(LPCWSTR filename)
{
    std::ifstream file;
    file.open(filename, std::ios::binary);

    BITMAPFILEHEADER bfh;
    file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));
    if (bfh.bfType != 0x4D42)
    {
        return E_FAIL;
    }

    BITMAPINFOHEADER bih;
    file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));
    if (bih.biBitCount != 32)
    {
        return E_FAIL;
    }

    file.seekg(bfh.bfOffBits);

    char r, g, b, a;

    file >> r >> g >> b >> a;

    std::vector<char> pixels(bih.biSizeImage);
    
    int pitch = bih.biWidth * (bih.biBitCount / 8);
   
    int index{};
    for (int y = bih.biHeight - 1; y >= 0; --y)
    {
        index = y * pitch;
        for (int x = 0; x < bih.biWidth; ++x)
        {
            char r{}, g{}, b{}, a{};
            file.read(&b, 1);
            file.read(&g, 1);
            file.read(&r, 1);
            file.read(&a, 1);

            if (r == (char)30 && g == (char)199 && b ==(char)250)
            {
                r = g = b = a = 0;
            }

            pixels[index++] = b;
            pixels[index++] = g;
            pixels[index++] = r;
            pixels[index++] = a;

        }
    }
    
    /* for (int y = bih.biHeight - 1; y >= 0; --y)
    {
        file.read(&pixels[y * pitch], pitch);
    }*/
    
  /*  file.read(&pixels[0],bih.biSizeImage);*/

    file.close();

    HRESULT hr;

    hr=mspRenderTarget->CreateBitmap(D2D1::SizeU(bih.biWidth, bih.biHeight), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), mspBitmap.GetAddressOf());  

    hr= mspBitmap->CopyFromMemory(nullptr, &pixels[0], pitch);
    ThrowIfFailed(hr);

    return S_OK;
}

HRESULT imageExample::LoadWICImage(LPCWSTR filename)
{
    HRESULT hr;

    //1.디코더 생성(Decoder)
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
    hr = mspWICFactory->CreateDecoderFromFilename(filename,
        nullptr,
        GENERIC_READ,
       WICDecodeMetadataCacheOnLoad,
        decoder.GetAddressOf()
        );
    ThrowIfFailed(hr);

    //2.디코더에서 프레임 획득
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
   ThrowIfFailed(decoder->GetFrame(0, frame.GetAddressOf()));

    //3.컨버터로 프레임에서 픽셀 배열을 구해와야함
   Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
   hr=mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
   ThrowIfFailed(hr);

  hr=converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);
  ThrowIfFailed(hr);

    //4.픽셀배열로 비트맵 생성
   hr = mspRenderTarget->CreateBitmapFromWicBitmap(
       converter.Get(), mspBitmap.GetAddressOf()
   );
   ThrowIfFailed(hr);
    return S_OK;
}
