#pragma once
#include <DirectXMath.h>

//�g�嗦�Ɋւ��鉉�Z�����������\����
struct Scale
{
	float x;
	float y;
	float z;

	//�Z�p���Z�q�̃I�[�o�[���[�h
	Scale operator *(const float i);
	Scale operator /(const float i);

	//������Z�q�̃I�[�o�[���[�h
	Scale& operator =(const DirectX::XMFLOAT3 fl);
	Scale& operator =(const Scale& fl);
	Scale& operator =(const float i);

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

	Scale() : x(1), y(1), z(1) {}
	Scale(DirectX::XMFLOAT3 fl) : x(fl.x), y(fl.y), z(fl.z) {}
	Scale(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};