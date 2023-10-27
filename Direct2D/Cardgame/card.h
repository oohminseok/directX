pragma once
#include "Image.h"
#include <memory>

enum class Type
{
	Dragon,
	Bear,
	Wolf
};

class Card
{
public:
	Type mType;
	HWND mHwnd;
	int mIndex;
	int mX;
	int mY;
	bool mFlip{};
	bool mClick{};

public:
	Image mpFront;
	Image mpBack;

public:
	Card(Type type,HWND hwnd,int index,int x,int y);

public:
	void Flip(bool flip);
	bool CheckedClick(POINT& pt);
	void Draw();
	void Release();

	Type GetType() { return mType; };
	int GetIndex() { return mIndex; };
};
