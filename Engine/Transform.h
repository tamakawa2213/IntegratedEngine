#pragma once
#include "Position.h"
#include "Scale.h"

using namespace DirectX;

class GameObject;

//�ʒu�A�����A�g�嗦�Ȃǂ��Ǘ�����N���X
class Transform
{
	XMMATRIX matTranslate_;	//�ړ��s��
	XMMATRIX matRotate_;	//��]�s��	
	XMMATRIX matScale_;		//�g��s��

public:
	Position position_;	//�ʒu
	XMFLOAT3 rotate_;	//����
	Scale scale_;		//�g�嗦
	Transform* pParent_;

	//�R���X�g���N�^
	Transform();

	//�f�X�g���N�^
	~Transform();

	//�e�s��̌v�Z
	void Calclation();

	//���[���h�s����擾
	XMMATRIX GetWorldMatrix();

	//�@���ό`�p�s����擾
	XMMATRIX GetNormalMatrix();

	//�w�肵�����W�Ɍ����ĉ�]������
	void LookAt(const XMFLOAT3& target);
};