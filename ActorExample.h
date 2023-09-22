#pragma once

#include "D2DFramework.h"
#include "Actor.h"
#include <list>

class ActorExample :
    public D2DFramework
{
    std::unique_ptr<Actor> mspBackground;
    std::list<std::unique_ptr<Actor>> mBugList;

public:
    HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title, 
        UINT width = 1024, UINT height = 768) override;
    void Release() override;
    void Render() override;

    void CheckBugs();
};

