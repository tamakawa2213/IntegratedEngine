#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize(int winW, int winH);

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(const XMVECTOR& position);
	void SetPosition(const XMFLOAT3& position);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(const XMVECTOR& target);
	void SetTarget(const XMFLOAT3& target);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	XMFLOAT3 GetCameraPosition();
	XMFLOAT3 GetCameraTarget();
	XMVECTOR GetCameraVecPosition();
	XMVECTOR GetCameraVecTarget();

	//�J�����̎����x�N�g�����擾
	XMVECTOR GetSight();

	//�v���W�F�N�V�����s����Z�b�g
	void SetProjMatrix(int winW, int winH, int split);

	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix();
};