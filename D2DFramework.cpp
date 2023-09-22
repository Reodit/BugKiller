#include "D2DFramework.h"
#include "BitmapManager.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HWND hwnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = gClassName;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProc;
    wc.cbSize = sizeof(WNDCLASSEX);
    if (!RegisterClassEx(&wc))
    {
        MessageBox(
            nullptr, L"Failed to register window class!", L"Error",
            MB_ICONEXCLAMATION | MB_OK
        );
        return E_FAIL;
    }

    RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd = CreateWindowEx(NULL,
        gClassName,
        L"Direct2D Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (hwnd == nullptr)
    {
        MessageBox(
            nullptr, L"Failed to create window class!", L"Error",
            MB_ICONEXCLAMATION | MB_OK);
        return E_FAIL;
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    mHwnd = hwnd;

    return S_OK;
}

HRESULT D2DFramework::InitD2D()
{
    HRESULT hr;

    hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.GetAddressOf());
    ThrowIfFailed(hr);

    return CreateDeviceResources();
}

HRESULT D2DFramework::CreateDeviceResources()
{
    RECT wr;
    GetClientRect(mHwnd, &wr);
    HRESULT hr = mspD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mHwnd, D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
        mspRenderTarget.GetAddressOf()
    );
    ThrowIfFailed(hr);
    
    return hr;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr;

    CoInitialize(nullptr);

    hr = InitWindow(hInstance, title, width, height);
    ThrowIfFailed(hr);

    hr = InitD2D();
    ThrowIfFailed(hr);

    hr = BitmapManager::Instance().Initialize(mspRenderTarget.Get());
    ThrowIfFailed(hr, "Failed To Create BitmapManager");

    ShowWindow(mHwnd, SW_SHOW);
    UpdateWindow(mHwnd);

    return hr;
}

void D2DFramework::Release()
{
    BitmapManager::Instance().Release();

    mspRenderTarget.Reset();
    mspD2DFactory.Reset();
    
    CoUninitialize();
}

void D2DFramework::Render()
{
    mspRenderTarget->BeginDraw();
    
    
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
    
    //TODO
    
    HRESULT hr = mspRenderTarget->EndDraw();

    if (hr = D2DERR_RECREATE_TARGET)
    {
        //Device Dependent Resource 다시 만들어야 함
        CreateDeviceResources();
    }

}

int D2DFramework::GameLoop()
{
    MSG msg{};
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Render();
        }
    }

    return static_cast<int>(msg.wParam);
}

LRESULT D2DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    D2DFramework* pFramework = reinterpret_cast<D2DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}