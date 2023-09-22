#include "ActorExample.h"
#include "Bug.h"

HRESULT ActorExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;
	hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr);

	mspBackground = std::make_unique<Actor>(this, L"Data/back.png", 0.0f, 0.0f);
	
	RECT rct{};
	GetClientRect(mHwnd, &rct);
	
	for (int i = 0; i < 40; ++i)
	{
		mBugList.push_back(std::make_unique<Bug>(this));
	}

	return S_OK;
}

void ActorExample::Release()
{
	for (auto& bug : mBugList)
	{
		bug.reset();
	}

	mBugList.clear();
	mspBackground.reset();

	D2DFramework::Release();
}

void ActorExample::Render()
{
	HRESULT hr;
	mspRenderTarget->BeginDraw();

	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	CheckBugs();

	mspBackground->Draw();
	for (auto& bug : mBugList)
	{
		bug->Draw();
	}

	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

void ActorExample::CheckBugs()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mHwnd, &pt);
		auto iter = std::remove_if(
			mBugList.begin(), mBugList.end(),
			[&](auto& actor)
			{
				Bug* pBug = static_cast<Bug*>(actor.get());
				pBug->IsClicked(pt);

				return pBug->mIsDeleted;
			}
		);

		mBugList.erase(iter, mBugList.end());
	}
}
