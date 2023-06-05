#include "BuggyHouse.h"

void BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	d2dframe::Initialize(hInstance, title, width, height);

	mspBackgroud = std::make_unique<actor>(this, L"data/back1_1024.png");
	

	for (int i = 0; i < 100; ++i)
	{
		mBugs.push_back(std::make_unique<Bug>(this));
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


	checkBugs();

	mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
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

void BuggyHouse::checkBugs()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000&&!mIsClicked)
	{
		mIsClicked = true;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mHwnd,&pt);

		for (auto& bug : mBugs)
		{
			if (bug->Clicekd(pt))
			{
				mBugs.remove(bug);
				break;
			}
		} 
	}

	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && mIsClicked)
	{
		mIsClicked = false;
	}
}
