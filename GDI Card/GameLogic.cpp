#include <vector>
#include <random>
#include "GameLogic.h"

void GameLogic::Init(HWND hwnd)
{
	mHwnd = hwnd;
	mBackground = std::make_unique<Gdiplus::Image>(L"data/bg_blank.png");

	CreateCards();
}

void GameLogic::Release()
{
	mDeck1.clear();
	mDeck2.clear();
	mBackground.reset();
}

void GameLogic::Draw(Gdiplus::Graphics& graphics)
{
	SetGraphics(graphics);
	graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

	for (auto& card : mDeck1)
	{
		card.Draw(graphics);
	}

	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
	Gdiplus::Font font{ L"맑은 고딕", 20 };
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	Gdiplus::PointF pos{ 900.0f, 150.0f };
	graphics.DrawString(L"1Player", -1, &font, pos, &brush);
	pos = { 910.0f, 20.0f };
	graphics.DrawString(L"점수", -1, &font, pos, &brush);
	pos = { 930.0f,50.0f };
	mGraphics->DrawString(std::to_wstring(mFlipCount1).c_str(), -1, &font, pos, &brush);

	pos = { 450,330.0f };
	graphics.DrawString(L"VS", -1, &font, pos, &brush);

	for (auto& card : mDeck2)
	{
		card.Draw(graphics);
	}

	pos = { 900.0f, 530.0f };
	graphics.DrawString(L"2Player", -1, &font, pos, &brush);
	pos={ 910.0f, 400.0f };
	graphics.DrawString(L"점수", -1, &font, pos, &brush);
	pos = { 930.0f,430.0f };
	mGraphics->DrawString(std::to_wstring(mFlipCount2).c_str(), -1, &font, pos, &brush);

}


void GameLogic::OnClick(int x, int y)
{
	Card* pCard1{};
	Card* pCard2{};

	if (mPlayer1 == false&&mPlayer2==true)
	{

		for (auto& card : mDeck1)
		{
			if (card.CheckClicked(x, y))
			{
				pCard1 = &card;
				break;
			}
		}

		if (pCard1)
		{
			RECT rect = { static_cast<LONG>(mCountRect1.GetLeft()),static_cast<LONG>(mCountRect1.GetTop()),static_cast<LONG>(mCountRect1.GetRight()),static_cast<LONG>(mCountRect1.GetBottom()) };
			


			if (mpSelectedCard1 == nullptr)
			{
				mpSelectedCard1 = pCard1;
			}
			else
			{
				if (mpSelectedCard1 == pCard1)
				{
					mpSelectedCard1 = nullptr;
				}
				else
				{
					if (pCard1->GetType() == mpSelectedCard1->GetType())
					{
						pCard1->Invalidate();

						mpSelectedCard1->Invalidate();

						mDeck1.remove_if([&](Card& card) {return card.GetIndex() == mpSelectedCard1->GetIndex(); });
						mDeck1.remove_if([&](Card& card) {return card.GetIndex() == pCard1->GetIndex(); });

						mpSelectedCard1 = nullptr;
						mFlipCount1++;
						InvalidateRect(mHwnd, &rect, false);
					}
					else
					{
						UpdateWindow(mHwnd);
						Sleep(500);
						pCard1->Flip(false);
						mpSelectedCard1->Flip(false);

						mpSelectedCard1 = nullptr;
						mPlayer1 = true;
						mPlayer2 = false;
					}

				}
			}
		}
	}
	else if(mPlayer1 == true && mPlayer2 == false)
	{
		for (auto& card : mDeck2)
		{
			if (card.CheckClicked(x, y))
			{
				pCard2 = &card;
				break;
			}
		}

		if (pCard2)
		{
			RECT rect = { static_cast<LONG>(mCountRect1.GetLeft()),static_cast<LONG>(mCountRect2.GetTop()),static_cast<LONG>(mCountRect2.GetRight()),static_cast<LONG>(mCountRect2.GetBottom()) };
			
			if (mpSelectedCard2 == nullptr)
			{
				mpSelectedCard2 = pCard2;
			}
			else
			{
				if (mpSelectedCard2 == pCard2)
				{
					mpSelectedCard2 = nullptr;
				}
				else
				{
					if (pCard2->GetType() == mpSelectedCard2->GetType())
					{
						pCard2->Invalidate();

						mpSelectedCard2->Invalidate();

						mDeck2.remove_if([&](Card& card) {return card.GetIndex() == mpSelectedCard2->GetIndex(); });
						mDeck2.remove_if([&](Card& card) {return card.GetIndex() == pCard2->GetIndex(); });
						mpSelectedCard2 = nullptr;
						mFlipCount2++;
						InvalidateRect(mHwnd, &rect, false);
						
					}
					else
					{
						UpdateWindow(mHwnd);
						Sleep(500);
						pCard2->Flip(false);
						mpSelectedCard2->Flip(false);

						mpSelectedCard2 = nullptr;
						mPlayer1 = false;
						mPlayer2 = true;
					}

				}
			}
		}
	}
	if (mDeck1.size() == 0)
	{
		MessageBox(NULL, L"1Player You Win", L"Win", MB_OK);
		CreateCards();
		RECT wr{0,0,1024,768};
		InvalidateRect(mHwnd, &wr, false);
		mFlipCount1 = 0;
	}
	else if (mDeck2.size() == 0)
	{
		MessageBox(NULL, L"2Player You Win", L"Win", MB_OK);
		CreateCards();
		RECT wr{ 0,0,1024,768 };
		InvalidateRect(mHwnd, &wr, false);
		mFlipCount2 = 0;
	}
}

void GameLogic::CreateCards()
{
	std::vector<Type> types;

	while (types.size() < static_cast<size_t>(BOARD_COLUMN * BOARD_ROW))
	{
		int modulaotr = types.size() % 6;
		switch (modulaotr)
		{
		case 0:
			types.push_back(Type::Bear);
			types.push_back(Type::Bear);
			break;

		case 2:
			types.push_back(Type::Wolf);
			types.push_back(Type::Wolf);
			break;

		case 4:
			types.push_back(Type::Dragon);
			types.push_back(Type::Dragon);
			break;
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(types.begin(), types.end(), g);

	int posX{ 15 }, posY{ 20 };
	int index = 0;
	for (int x = 0; x<BOARD_ROW; ++x)
	{
		posY = 20;
		for (int y = 0; y < BOARD_COLUMN; ++y)
		{
			mDeck1.push_back(Card(mHwnd,index,types[index++], posX, posY));
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
			mDeck2.push_back(Card(mHwnd, index2, types[index2++], posX2, posY2));
			posY2 += 140 + 10;
		}
		posX2 += 100 + 10;
	}

}
