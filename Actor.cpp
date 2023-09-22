#include "Actor.h"
#include "BitmapManager.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) : 
	mpFramework { pFramework },
	mX{ }, mY{ }, mOpacity{ 1.0f }
{
	mpBitmap = BitmapManager::Instance().LoadBitmap(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename)
{
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor()
{
}

void Actor::Draw(float x, float y, float opacity)
{
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rct{
		x,
		y,
		static_cast<float>(x + size.width),
		static_cast<float>(y + size.height)
	};

	auto pRenderTarget{ mpFramework->GetRenderTarget() };

	pRenderTarget->DrawBitmap(
		mpBitmap,
		rct,
		opacity
	);
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}
