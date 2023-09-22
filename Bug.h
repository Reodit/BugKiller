#pragma once
#include "Actor.h"

class Bug :
    public Actor
{
    const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };
    
    int mSteps;
    float mRotation;

public:
    bool mIsDeleted;
    bool isClicked;

public:
    Bug(D2DFramework* pFramework);

    virtual void Draw() override;

    bool IsClicked(POINT& pt);
};

