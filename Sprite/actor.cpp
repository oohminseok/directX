#include "actor.h"

actor::actor(d2dframe* pFramework, std::wstring filename):
    actor(pFramework,filename,0.0f,0.0f,1.0f)
{
}

actor::actor(d2dframe* pFramework, std::wstring filename, float x, float y, float opacity):mpFramework{pFramework},mX{x},mY{y},mOpacity{opacity}
{
  
  mpBitmap= bitmapmanager::Instance().LoadBitmapW(filename);
}

actor::~actor()
{
    
}

void actor::Draw(float x, float y, float opacity)
{
    auto pRenderTarget = mpFramework->GetRendetTarget();
    auto size = mpBitmap->GetPixelSize();
    D2D1_RECT_F rect{
        x,y,
        x + size.width,y + size.height
    };

    pRenderTarget->DrawBitmap(mpBitmap, rect, opacity);
}


void actor::Draw()
{
    Draw(mX, mY, mOpacity);
}
