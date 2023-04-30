#include "RayCastData.h"
#include "Camera.h"
#include "Direct3D.h"
#include "Input.h"

void RayCastData::CreateMouseRay()
{
	//�r���[�|�[�g�s��
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

	//�e�s��̋t�s��
	XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
	XMMATRIX invPrj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

	XMMATRIX mat = invVp * invPrj * invView;


	XMINT3 mousePosFront{}, mousePosBack{};

	{
		XMINT3 mousePosition = Input::Mouse::GetPosition();

		//���ʂȂ�x���W���X�N���[���̉����ŌJ��Ԃ�����
		if (Direct3D::SplitScrMode == SCREEN_MODE::SPLIT_2)
			mousePosition.x = mousePosition.x % (int)(Direct3D::scrWidth / 2.0f);

		mousePosFront = mousePosBack = mousePosition;
	}

	//�}�E�X�̃��C�̎�O�����쐬
	mousePosFront.z = 0;
	XMVECTOR vPos = XMLoadSInt3(&mousePosFront);
	vPos = XMVector3TransformCoord(vPos, mat);
	XMVECTOR front = vPos;

	//�������쐬
	mousePosBack.z = 1;
	vPos = XMLoadSInt3(&mousePosBack);
	vPos = XMVector3TransformCoord(vPos, mat);
	XMVECTOR back = vPos;

	//���C���쐬
	XMVECTOR ray = back - front;
	ray = XMVector3Normalize(ray);

	XMStoreFloat3(&start, front);
	XMStoreFloat3(&dir, ray);
}
