#include "actorexample.h"

void BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	d2dframe::Initialize(hInstance, title, width, height);

	mspBackgroud = std::make_unique<actor>(this, L"data/back1_1024.png");
	
	for (int i = 0; i < 100; ++i)
	{
		mBugs.push_back(std::make_unique<actor>(this, L"data/bug.png",i,i%30*30));
	}
	

}

void BuggyHouse::Release()
{
	mBugs.clear();
	mspBackgroud.reset();

	d2dframe::Release();
}

void BuggyHouse::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspBackgroud->Draw();
	
	for (auto& bug : mBugs)
	{
		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		
	}


}
