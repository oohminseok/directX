#pragma once
#include <list>
#include <vector>
#include "d2dframe.h"
#include "actor.h"
#include "Bug.h"

class BuggyHouse :public d2dframe
{
	std::unique_ptr<actor> mspBackgroud;
	std::list<std::unique_ptr<Bug>> mBugs;

private:
	bool mIsClicked{ false };

public:
	virtual void Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"actor example", UINT width = 1024, UINT height = 768)override;
	virtual void Release() override;
	virtual void Render() override;

	void checkBugs();
};

