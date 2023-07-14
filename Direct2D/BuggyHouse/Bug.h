#pragma once
#include "actor.h"
class Bug :
    public actor
{
   /* enum class Direction
    {
        UP,
        UP_RIGHT,
        RIGHT,
        DOWN_RIGHT,
        DOWN,
        DOWN_LEFT,
        LEFT,
        UP_LEFT,

        COUNT
    };*/

    /*Direction mDirection;*/

    const D2D_POINT_2F UPVECTOR{ 0.0f,-1.0f };
    float mRotation;
    float mSteps;

public:
    Bug(d2dframe* pFramework);

    virtual void Draw() override;

    bool Clicekd(POINT& pt);


};

