#include <string>
#include <vector>
#include <random>
#include "GameLogic.h"

void solitare::GameLogic::Initialize(HWND hwnd)
{
	mHwnd = hwnd;
	mBackground = std::make_unique<Gdiplus::Image>(L"data/bg_blank.png");
	CreateCards();
}

void solitare::GameLogic::Release()
{
	mDeck1.clear();
	mDeck2.clear();
	mBackground.reset();
}

void solitare::GameLogic::Draw(Gdiplus::Graphics& graphics)
{
	graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

	for (auto& card : mDeck1)
	{
		card.Draw(graphics);
	}


	Gdiplus::PointF pos(895.0f, 20.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
	Gdiplus::Font font(L"맑은 고딕", 20);

	graphics.DrawString(L"클릭수", -1, &font, pos, &brush);

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	
	
}

void solitare::GameLogic::Draw2(Gdiplus::Graphics& graphics)
{
	for (auto& card : mDeck2)
	{
		card.Draw(graphics);
	}
}

void solitare::GameLogic::OnClick(int x, int y)
{
	Solitaire::Card* pCard{};
	for (auto& card : mDeck1)
	{
		if (card.checkClicked(x,y))
		{
			pCard = &card;
			break;
		}
	}

	if (pCard)
	{
		mFlipCount++;
		RECT rct = { static_cast<LONG>(COUNT_RECT.GetLeft()),static_cast<LONG>(COUNT_RECT.GetTop()),static_cast<LONG>(COUNT_RECT.GetRight()),static_cast<LONG>(COUNT_RECT.GetBottom()) };
		InvalidateRect(mHwnd, &rct,false);

		
		if (mpSelectedCard == nullptr)
		{
			mpSelectedCard = pCard;
		}
		else
		{
			if (mpSelectedCard == pCard)
			{
				mpSelectedCard = nullptr;
			}
			if (pCard->GetType() == mpSelectedCard->GetType())
			{
				pCard->Invalidate();
				mpSelectedCard->Invalidate();

				mDeck1.remove_if([&](Solitaire::Card& card)
					{
						return card.GetIndex() == mpSelectedCard->GetIndex();
					}
				);
				mDeck1.remove_if([&](Solitaire::Card& card)
					{
						return card.GetIndex() == pCard->GetIndex();
					}
				);
				mpSelectedCard = nullptr;
			}
			else
			{
				UpdateWindow(mHwnd);
				Sleep(500);
				pCard->Flip(false);
				mpSelectedCard->Flip(false);

				mpSelectedCard = nullptr;
			}
		}
	}

}

void solitare::GameLogic::CreateCards()
{
	//TODO
	std::vector<Solitaire::Type> types;
	while (types.size() < BOARD_ROW * BOARD_COLUMN)
	{
		int mod = types.size() % 6;

		switch (mod)
		{
		case 0:
			types.push_back(Solitaire::Type::Bear);
			types.push_back(Solitaire::Type::Bear);
			break;

		case 2:
			types.push_back(Solitaire::Type::Wolf);
			types.push_back(Solitaire::Type::Wolf);
			break;

		case 4:
			types.push_back(Solitaire::Type::Dragon);
			types.push_back(Solitaire::Type::Dragon);
			break;

		default:
			OutputDebugString(L"카드 생성에 문제가 있음!");
			break;
		}
	}
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(types.begin(), types.end(), g);

	int posX{ 15 }, posY{ 10 };
	int index = 0;
	for (int x = 0; x < BOARD_ROW; ++x)
	{
		posY = 10;
		for (int y = 0; y < BOARD_COLUMN; ++y)
		{
			mDeck1.push_back(Solitaire::Card(mHwnd, index, types[index++], posX, posY));
			posY += 140 + 10;
		}
		posX += 100 + 10;
	}

	std::shuffle(types.begin(), types.end(), g);

	int posX2{ 15 }, posY2{ 400 };
	int index2 = 0;
	for (int x = 0; x < BOARD_ROW; ++x)
	{
		posY2 = 400;
		for (int y = 0; y < BOARD_COLUMN; ++y)
		{
			mDeck2.push_back(Solitaire::Card(mHwnd, index2, types[index2++], posX2, posY2));
			posY2 += 140 + 10;
		}
		posX2 += 100 + 10;
	}
	
	
	
}
