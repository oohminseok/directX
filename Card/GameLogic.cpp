#include <vector>
#include <random>
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;
		mBackground = std::make_unique<Gdiplus::Image>(L"data/bg_blank.png");

		CreateCards();
	}
	void GameLogic::Release()
	{
		mDeck.clear();

		mBackground.reset();
	}
	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(),0,0,mBackground->GetWidth(),mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		Gdiplus::PointF pos(895.0f, 20.0f);

		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		Gdiplus::Font font(L"¸¼Àº °íµñ", 20);

		graphics.DrawString(L"Å¬¸¯¼ö:", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		Gdiplus::PointF pos2(930.0f, 40.0f);
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, pos2, &brush);

	}
	void GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};

		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				pCard = &card;
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;
			RECT rct = { static_cast<LONG>(mCountRect.GetLeft()),
			static_cast<LONG>(mCountRect.GetTop()),
			static_cast<LONG>(mCountRect.GetRight()),
			static_cast<LONG>(mCountRect.GetBottom()) };
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelcetedCard == nullptr)
			{
				mpSelcetedCard = pCard;
			}
			else
			{
				if (mpSelcetedCard == pCard)
				{
					mpSelcetedCard = nullptr;
				}
				else
				{
					if (pCard->GetType() == mpSelcetedCard->GetType())
					{
						pCard->Invalidate();
						mpSelcetedCard->Invalidate();
						mDeck.remove_if([&](Card& card)
							{
								return card.GetIndex() == mpSelcetedCard->GetIndex();
							});
						mDeck.remove_if([&](Card& card)
							{
								return card.GetIndex() == pCard->GetIndex();
							});
						mpSelcetedCard = nullptr;
					}
					else
					{
						UpdateWindow(mHwnd);
						Sleep(500);
						pCard->Flip(false);
						mpSelcetedCard->Flip(false);

						mpSelcetedCard = nullptr;
					}
				}
			}


		}

	}
	void GameLogic::CreateCards()
	{
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_COLUMN * BOARD_ROW))
		{
			int modulator = types.size() % 6;
			switch (modulator)
			{
			case 0:
				types.push_back(Type::BEAR);
				types.push_back(Type::BEAR);
				break;

			case 2:
				types.push_back(Type::WOLF);
				types.push_back(Type::WOLF);
				break;

			case 4:
				types.push_back(Type::DRAGON);
				types.push_back(Type::DRAGON);
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
				mDeck.push_back(Card(mHwnd,index, types[index++], posX, posY));
				posY += 140 + 10;
			}
			posX += 100 + 10;
		}
	}
}