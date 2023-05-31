#pragma once
#include <vector>
#include "d2dframe.h"
#include "actor.h"
class BuggyHouse :public d2dframe
{
	std::unique_ptr<actor> mspBackgroud;
	std::vector<std::unique_ptr<actor>> mBugs;

public:
	virtual void Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"actor example", UINT width = 1024, UINT height = 768)override;
	virtual void Release() override;
	virtual void Render() override;
};

