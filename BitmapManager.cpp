#include "BitmapManager.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(wstring filename, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr{};

	// 1. 디코더 생성
	ComPtr<IWICBitmapDecoder> bitmapDecoder;
	hr = mspWICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. 디코더에서 0번 프레임을 가져와야 함
	ComPtr<IWICBitmapFrameDecode> frame;
	hr = bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. Converter를 사용해 픽셀 포멧을 변환
	ComPtr<IWICFormatConverter> converter;
	hr = mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// 4. 비트맵 픽셀 배열
	hr = mpRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr)
	{
		return E_FAIL;
	}

	mpRenderTarget = pRT;

	HRESULT hr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(mspWICFactory.GetAddressOf()));
	
	ThrowIfFailed(hr);

	return hr;
}

void BitmapManager::Release()
{
	for (auto& e : mBitmapResources)
	{
		e.second.Reset();
	}
	mspWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmap(wstring filename)
{
	// map에 해당 filename이 있는가?
		// 없으면 : 최초 로딩 -> WIC Loading -> ID2D1Bitmap -> map
		// 있으면 : 이미 로딩 되어 있음 -> map

	if (mspWICFactory == nullptr)
	{
		return nullptr;
	}

	auto result = mBitmapResources.insert(pair<wstring, ComPtr<ID2D1Bitmap>>(filename, nullptr));
	
	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());

		result.first->second = spBitmap;
	}

	return result.first->second.Get();
}
