#include "BitmapManager.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(wstring filename, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr{};

	// 1. ���ڴ� ����
	ComPtr<IWICBitmapDecoder> bitmapDecoder;
	hr = mspWICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. ���ڴ����� 0�� �������� �����;� ��
	ComPtr<IWICBitmapFrameDecode> frame;
	hr = bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. Converter�� ����� �ȼ� ������ ��ȯ
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

	// 4. ��Ʈ�� �ȼ� �迭
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
	// map�� �ش� filename�� �ִ°�?
		// ������ : ���� �ε� -> WIC Loading -> ID2D1Bitmap -> map
		// ������ : �̹� �ε� �Ǿ� ���� -> map

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
