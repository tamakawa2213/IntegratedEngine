#include "Camera.h"
#include "CallDef.h"

//�ϐ�
namespace Camera {
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;	//�r���[�s��
	XMMATRIX projMatrix_;	//�v���W�F�N�V�����s��
	XMMATRIX billboard_;

//������
	void Initialize(int winW, int winH)
	{
		position_ = XMVectorSet(0, 5, 1.4f, 0);	//�J�����̈ʒu
		target_ = XMVectorSet(0, 0, 1.5f, 0);		//�J�����̏œ_
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)winW / (FLOAT)winH / 2.0f, 0.1f, 1000.0f);
	}

	//�X�V
	void Update()
	{
		//�r���[�s��̍쐬(�J�����Œ�̃Q�[���Ȃ�Initialize�ɏ���)
		viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

		billboard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, XMVectorSet(0, 1, 0, 0));
		billboard_ = XMMatrixInverse(nullptr, billboard_);
	}

	//�ʒu��ݒ�
	void SetPosition(XMVECTOR position)
	{
		position_ = position;
	}

	void SetPosition(XMFLOAT3 position)
	{
		position_ = XMLoadFloat3(&position);
	}

	//�œ_��ݒ�
	void SetTarget(XMVECTOR target)
	{
		target_ = target;
	}

	void SetTarget(XMFLOAT3 target)
	{
		target_ = XMLoadFloat3(&target);
	}

	//�r���[�s����擾
	XMMATRIX GetViewMatrix()
	{
		return viewMatrix_;
	}

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix()
	{
		return projMatrix_;
	}
	XMFLOAT3 GetCameraPosition()
	{
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, position_);
		return pos;
	}
	XMFLOAT3 GetCameraTarget()
	{
		XMFLOAT3 tar;
		XMStoreFloat3(&tar, target_);
		return tar;
	}
	XMVECTOR GetCameraVecPosition()
	{
		return position_;
	}
	XMVECTOR GetCameraVecTarget()
	{
		return target_;
	}

	XMVECTOR GetSight()
	{
		return target_ - position_;
	}

	void SetProjMatrix(int winW, int winH, int split)
	{
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)winW / (FLOAT)winH / split, 0.1f, 1000.0f);
	}


	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix()
	{
		return billboard_;
	}
}