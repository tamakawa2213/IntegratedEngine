#pragma once
#include <DirectXMath.h>

struct RayCastData
{
	DirectX::XMFLOAT3 start;	//���C�̎n�_
	DirectX::XMFLOAT3 dir;		//���˂������
	float dist;					//�n�_�Ɩ��������ʒu�̋���
	bool hit;					//�����������ǂ���
	DirectX::XMFLOAT3 hitpos;	//���������ʒu

	void CreateMouseRay();	//�}�E�X�̃��C���쐬����

	RayCastData() : start(), dir(), dist(9999.0f), hit(false), hitpos() {}
};