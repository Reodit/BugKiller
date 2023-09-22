#pragma once

#include <wincodec.h>
#include <d2d1.h>
#include <wrl/client.h>
#include <string>
#include <sstream>
#include "com_exception.h"

class D2DFramework
{
private:
	const LPCWSTR gClassName { L"MyWindowClass" };

protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget;
	HWND mHwnd{ nullptr };

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();
	virtual HRESULT CreateDeviceResources();

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width = 1024, UINT height = 768);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	ID2D1HwndRenderTarget* GetRenderTarget() const
	{
		return mspRenderTarget.Get();
	}
	HWND GetWindowHandle() const
	{
		return mHwnd;
	}
};

