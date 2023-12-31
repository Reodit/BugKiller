#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <map>
#include <string>
#include "com_exception.h"

class BitmapManager final
{
private:
	ID2D1HwndRenderTarget* mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources{};

public:
	static BitmapManager& Instance()
	{
		static BitmapManager instance;
		return instance;
	}

private:
	BitmapManager() = default;
	BitmapManager(const BitmapManager&) = delete;
	void operator = (const BitmapManager&) = delete;

public:
	~BitmapManager() = default;

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();

	ID2D1Bitmap* LoadBitmap(std::wstring filename);
};