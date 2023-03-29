#pragma once
#include <DirectXMath.h>

//�g�嗦�Ɋւ��鉉�Z�����������N���X
class Scale
{
public:
	Scale();
	~Scale();

	float x;
	float y;
	float z;

	//�Z�p���Z�q�̃I�[�o�[���[�h
	Scale operator *(const float i);
	Scale operator /(const float i);

	//������Z�q�̃I�[�o�[���[�h
	Scale& operator =(const DirectX::XMFLOAT3 fl);
	Scale& operator =(const Scale& fl);

	//����������Z�q�̃I�[�o�[���[�h
	Scale operator *=(const float i);
	Scale operator /=(const float i);

	//��r���Z�q�̃I�[�o�[���[�h
	bool operator ==(const DirectX::XMFLOAT3 fl) const;
	bool operator ==(const Scale& fl) const;
	bool operator !=(const DirectX::XMFLOAT3 fl) const;
	bool operator !=(const Scale& fl) const;

	//XMFLOAT3�ɃL���X�g�\�ɂ���
	operator DirectX::XMFLOAT3() const;
};