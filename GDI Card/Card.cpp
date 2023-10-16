#include "Card.h"

namespace solitaire
{
    Card::Card(HWND hwnd, int index, Type type, int x, int y) :mHwnd{ hwnd }, mIndex{ index }, mType{ type }, mX{ x }, mY{ y }, mIsFront{ false }
    {
        std::wstring filename;

        switch (type)
        {
        case Type::BEAR:
            filename = L"data/card_creature_bear.png";
            break;

        case Type::WOLF:
            filename = L"data/card_creature_wolf.png";
            break;

        case Type::DRAGON:
            filename = L"data/card_creature_dragon.png";
            break;
        }

        mBack = std::make_unique<Gdiplus::Image>(L"data/card_back.png");
        mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
    }

    bool Card::CheckClicked(int x, int y)
    {
        /*if (x > mX && y >= mY && static_cast<UINT>(x) <= mX + mFront->GetWidth() &&
            static_cast<UINT>(y) <= mY + mFront->GetHeight())*/

        Gdiplus::Rect rct{ mX,mY,static_cast<INT>(mFront->GetWidth()), 
            static_cast<INT>(mFront->GetHeight()) };
        if (rct.Contains(x, y))
        {
            Flip(!mIsFront);

            return true;
        }

        return false;
    }

    void Card::Flip(bool isFront)
    {
        mIsFront = isFront;
        Invalidate();
    }

    void Card::Draw(Gdiplus::Graphics& graphics)
    {
        if (mIsFront)
        {
            graphics.DrawImage(
                mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
        }
        else
        {
            graphics.DrawImage(
                mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
        }
    }
    void Card::Invalidate()
    {
        RECT rct{ mX,mY,
            static_cast<LONG>(mX + mBack->GetWidth()),
            static_cast<LONG>(mY + mBack->GetHeight()) };

        InvalidateRect(mHwnd, &rct, false);
    }
}
