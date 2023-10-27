#pragma once
#include <list>
#include "Card.h"

namespace solitare
{
	class GameLogic
	{
		const int BOARD_ROW = 8;
		const int BOARD_COLUMN = 1;
		const Gdiplus::Rect COUNT_RECT{ 885.0f,60.0f,120,30.0f };

	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Solitaire::Card> mDeck1;
		std::list<Solitaire::Card> mDeck2;
		int mFlipCount;
		Solitaire::Card* mpSelectedCard{};

		HWND mHwnd;

	public:
		void Initialize(HWND hwnd);
		void Release();

		void Draw(Gdiplus::Graphics& graphics);
		void Draw2(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);
		 
	private:
		void CreateCards();

	};
}

