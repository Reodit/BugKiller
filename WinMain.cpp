#include <windows.h>
#include "ActorExample.h"

//class CardGame : public D2DFramework
//{
//    // coding
//};
//
//CardGame myFramework;
ActorExample myFramework;

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)

{
    int ret{};

    try
    {
        myFramework.Initialize(hInstance, L"D2DFramework");

        ret = myFramework.GameLoop();

        myFramework.Release();
    }

    catch (const com_exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    myFramework.Release();

    return ret;
}