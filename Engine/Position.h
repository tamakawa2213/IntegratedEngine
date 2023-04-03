#pragma once
#include <DirectXMath.h>

//�ʒu�Ɋւ��鉉�Z�����������\����
struct Position
{
	float x;
	float y;
	float z;

	//�Z�p���Z�q�̃I�[�o�[���[�h
	Position operator +(DirectX::XMFLOAT3 fl);
	Position operator +(Position fl);
	Position operator -(DirectX::XMFLOAT3 fl);
	Position operator -(Position fl);

	//������Z�q�̃I�[�o�[���[�h
	Position& operator =(const DirectX::XMFLOAT3 fl);
	Position& operator =(const Position& fl);

	//����������Z�q�̃I�[�o�[���[�h
	Position& operator +=(DirectX::XMFLOAT3 fl);
	Position& operator +=(Position& fl);
	Position& operator -=(DirectX::XMFLOAT3 fl);
	Position& operator -=(Position& fl);

	//��r���Z�q�̃I�[�o�[���[�h
	bool operator ==(const DirectX::XMFLOAT3 fl) const;
	bool operator ==(const Position& fl) const;
	bool operator !=(const DirectX::XMFLOAT3 fl) const;
	bool operator !=(const Position& fl) const;

	//XMFLOAT3�ɃL���X�g�\�ɂ���
	operator DirectX::XMFLOAT3() const;

	Position() : x(0), y(0), z(0) {}
	Position(DirectX::XMFLOAT3 fl) : x(fl.x), y(fl.y), z(fl.z) {}
	Position(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};