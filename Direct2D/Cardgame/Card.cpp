#include <string>
#include "Card.h"

namespace Solitaire
{
	Card::Card(HWND hwnd, int index, Solitaire::Type type, unsigned int x, unsigned int y) :mHwnd{ hwnd }, mIndex{index}, mType{type}, mX{x}, mY{y}, mIsFront{false}
	{
		std::wstring filename;

		switch (type)
		{
		case Type::Wolf:
			filename = L"data/card_creature_wolf.png";
			break;
		case Type::Dragon:
			filename = L"data/card_creature_dragon.png";
			break;
		case Type::Bear:
			filename = L"data/card_creature_bear.png";
		}

		mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
		mFront = std::make_unique<Gdiplus::Image>(filename.c_str());

		
	}

	bool Card::checkClicked(unsigned int x, unsigned int y)
	{
		if (x >= mX && x <= mX + mFront->GetWidth() &&
			y >= mY && y < mY + mFront->GetHeight())
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
			graphics.DrawImage(mFront.get(), static_cast<int>(mX), static_cast<int>(mY));
		}
		else
		{
			graphics.DrawImage(mBack.get(), static_cast<int>(mX), static_cast<int>(mY));
		}
	}
	void Card::Invalidate()
	{
		RECT rect{ static_cast<LONG>(mX),static_cast<LONG>(mY),static_cast<LONG>(mX + mFront->GetWidth()),static_cast<LONG>(mY + mFront->GetHeight()) };
		InvalidateRect(mHwnd, &rect, false);
	}
}
