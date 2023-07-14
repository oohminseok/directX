#pragma once

#include "d2dframe.h"

class actor
{
public:
	d2dframe* mpFramework;
	ID2D1Bitmap* mpBitmap;

	float mX;
	float mY;
	float mOpacity;

public:
	actor() = delete;
	actor(d2dframe* pFramework, std::wstring filename);
	actor(d2dframe* pFramework, std::wstring filename,float x,float y,float opacity=1.0f);
	virtual ~actor();

protected:
	void Draw(float x, float y, float opacity=1.0f);


public:
	virtual void Draw();

};

