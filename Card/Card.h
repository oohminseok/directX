#pragma once
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <gdiplus.h>

namespace solitaire
{
	enum class Type
	{
		WOLF,
		DRAGON,
		BEAR
	};


	class Card
	{
	private:
		std::unique_ptr<Gdiplus::Image> mFront;
		std::unique_ptr<Gdiplus::Image> mBack;

		HWND mHwnd;
		int mIndex;
		Type mType;
		int mX;
		int mY;
		bool mIsFront;

	public:
		Card(HWND hwnd,int index,Type type, int x, int y);

		bool CheckClicked(int x, int y);
		void Flip(bool isFront);
		void Draw(Gdiplus::Graphics& graphics);
		void Invalidate();

		Type GetType()
		{
			return mType;
		}
		int GetIndex()
		{
			return mIndex;
		}
	};
}

