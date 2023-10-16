#pragma once
#include <list>
#include "Card.h"

class GameLogic
{
	const int BOARD_ROW = 8;
	const int BOARD_COLUMN = 2;
	const Gdiplus::RectF mCountRect1{ 885.0f,60.0f,120.0f,30.0f };
	const Gdiplus::RectF mCountRect2{ 885.0f,440.0f,120.0f,30.0f };

private:
	std::unique_ptr<Gdiplus::Image> mBackground;
	std::list<Card> mDeck1;
	std::list<Card> mDeck2;

	HWND mHwnd{};
	Gdiplus::Graphics* mGraphics{};
	int mFlipCount1{};
	int mFlipCount2{};
	bool mPlayer1{ false };
	bool mPlayer2{ true };
	Card* mpSelectedCard1{};
	Card* mpSelectedCard2{};

public:
	void Init(HWND hwnd);
	void Release();
	void Draw(Gdiplus::Graphics& graphics);
	void OnClick(int x, int y);

private:
	void CreateCards();
};
