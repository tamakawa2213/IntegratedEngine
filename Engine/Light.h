#pragma once
#include<DirectXMath.h>

using namespace DirectX;

class Light
{
	XMVECTOR pos;	//�����̈ʒu�x�N�g���܂��͕����x�N�g��
public:
	Light();
	~Light();
	void SetLight(XMVECTOR update);
	XMFLOAT4 GetLight();
};