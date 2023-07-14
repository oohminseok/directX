#include "Bug.h"

Bug::Bug(d2dframe* pFramework):actor(pFramework,L"data/bug.png")
{
	RECT rct;
	GetClientRect(pFramework->GetWindowHandle(), &rct);
	mX =static_cast<float>(rct.left+ rand() % (rct.right - rct.left));
	mY = static_cast<float>(rct.top + rand() % (rct.bottom - rct.top));

	mRotation = 0.0f;
	mSteps = 0.0f;

}

void Bug::Draw()
{
	///*if (mSteps++ > 20)
	//{
	//	mSteps = 0;

	//	int dir = static_cast<int>(mDirection);
	//	dir += (1 - rand() % 3);

	//	int count = static_cast<int>(Direction::COUNT);
	//	dir = (dir + count) % count;

	//	mDirection = static_cast<Direction>(dir);

	//}

	if (mSteps++ > 20)
	{
		mSteps = 0;
		mRotation += (1 - rand() % 3) * 45;
	}


	/*switch (mDirection)
	{
		case Direction::UP:
			mY--;
			break;
		case Direction::UP_RIGHT:
			mY--;
			mX++;
		case Direction::RIGHT:
			mX++;
			break;
		case Direction::DOWN_RIGHT:
			mY++;
			mX++;
			break;
		case Direction::DOWN:
			mY++;
			break;
		case Direction::DOWN_LEFT:
			mY++;
			mX--;
			break;
		case Direction::LEFT:
			mX--;
			break;
		case Direction::UP_LEFT:
			mY--;
			mX--;
			break;
	}*/

	auto size{ mpBitmap->GetPixelSize() };

	auto target=UPVECTOR* D2D1::Matrix3x2F::Rotation(mRotation);
	mX += target.x;
	mY += target.y;

	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation,D2D1_POINT_2F{size.width*0.5f,size.height*0.5f});

	auto pRT = mpFramework->GetRendetTarget();

	pRT->SetTransform(matRotation * matTranslate);
	
	D2D1_RECT_F rect
	{
		0, 0,
		static_cast<float>(size.width),
		static_cast<float>(size.height),
	};
	
	pRT->DrawBitmap(mpBitmap, rect, mOpacity);
	

}

bool Bug::Clicekd(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };

	if (pt.x >= mX && pt.x <= mX + size.width &&
		pt.y >= mY && pt.y <= mY + size.height)
	{
		return true;
	}

	return false;
}
