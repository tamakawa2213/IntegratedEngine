#include "RayCastData.h"
#include "Camera.h"
#include "Direct3D.h"
#include "Input.h"

void RayCastData::CreateMouseRay()
{
	//ビューポート行列
	float w = 0;
	switch (Direct3D::SplitScrMode)
	{
	case SCREEN_MODE::FULL: w = (float)Direct3D::scrWidth / 2.0f; break;
	case SCREEN_MODE::SPLIT_2: w = (float)Direct3D::scrWidth / 4.0f; break;
	default: break;
	}
	float h = (float)Direct3D::scrHeight / 2.0f;

	XMMATRIX vp =
	{
		w, 0, 0, 0,
		0,-h, 0, 0,
		0, 0, 1, 0,
		w, h, 0, 1
	};

	//各行列の逆行列
	XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
	XMMATRIX invPrj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

	XMMATRIX mat = invVp * invPrj * invView;


	XMINT3 mousePosFront{}, mousePosBack{};

	{
		XMINT3 mousePosition = Input::Mouse::GetPosition();

		//二画面ならx座標をスクリーンの横幅で繰り返させる
		if (Direct3D::SplitScrMode == SCREEN_MODE::SPLIT_2)
			mousePosition.x = mousePosition.x % (int)(Direct3D::scrWidth / 2.0f);

		mousePosFront = mousePosBack = mousePosition;
	}

	//マウスのレイの手前側を作成
	mousePosFront.z = 0;
	XMVECTOR vPos = XMLoadSInt3(&mousePosFront);
	vPos = XMVector3TransformCoord(vPos, mat);
	XMVECTOR front = vPos;

	//奥側を作成
	mousePosBack.z = 1;
	vPos = XMLoadSInt3(&mousePosBack);
	vPos = XMVector3TransformCoord(vPos, mat);
	XMVECTOR back = vPos;

	//レイを作成
	XMVECTOR ray = back - front;
	ray = XMVector3Normalize(ray);

	XMStoreFloat3(&start, front);
	XMStoreFloat3(&dir, ray);
}
