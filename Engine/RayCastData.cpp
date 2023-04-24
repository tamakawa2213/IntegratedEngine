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

	//マウスのレイの手前側を作成
	XMFLOAT3 mousePosFront = Input::Mouse::GetPosition();
	mousePosFront.x = mousePosFront.x;
	mousePosFront.z = 0;

	XMVECTOR vPos = XMLoadFloat3(&mousePosFront);

	vPos = XMVector3TransformCoord(vPos, mat);

	XMVECTOR front = vPos;

	//奥側を作成
	XMFLOAT3 mousePosBack = Input::Mouse::GetPosition();
	mousePosBack.x = mousePosBack.x;
	mousePosBack.z = 1.0f;

	vPos = XMLoadFloat3(&mousePosBack);

	vPos = XMVector3TransformCoord(vPos, mat);

	XMVECTOR back = vPos;

	XMVECTOR ray = back - front;
	ray = XMVector3Normalize(ray);

	XMStoreFloat3(&start, front);
	XMStoreFloat3(&dir, ray);
}
