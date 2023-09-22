#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Data/bug.png")
{
	mRotation = 0.0f;
	mSteps = 0;

	RECT rct;
	GetClientRect(mpFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));
}

void Bug::Draw()
{
	// 5����
	if (mSteps++ > 5)
	{
		//���� ��ȯ
		mSteps = 0;
		mRotation += (rand() % 3 - 1) * 45.0f;
	}

	// ����
	auto direction = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += direction.x;
	mY += direction.y;

	//�׸��� �ڵ带 ��ķ� ��ü
	auto size{ mpBitmap->GetPixelSize() };

	// �̵�
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);

	// ȸ��
	auto matRotate = D2D1::Matrix3x2F::Rotation(mRotation, D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });

	// ũ�� ����
	//Transform - �����̵�, ȸ��, ũ������;

	mpFramework->GetRenderTarget()->SetTransform(matRotate * matTranslate);

	D2D1_RECT_F rect{
		0.0f, 0.0f,
		static_cast<float>(size.width),
		static_cast<float>(size.height)
	};

	mpFramework->GetRenderTarget()->DrawBitmap(
		mpBitmap,
		rect,
		mOpacity

	);
}

bool Bug::IsClicked(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{
		mX, mY,
		static_cast<float>(mX + size.width),
		static_cast<float>(mY + size.height)
	};

	if (pt.x >= rect.left &&
		pt.x <= rect.right &&
		pt.y >= rect.top &&
		pt.y <= rect.bottom)
	{
		mIsDeleted = true;
		return true;
	}

	return false;
}
