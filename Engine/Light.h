#pragma once
#include<DirectXMath.h>

using namespace DirectX;

class Light
{
	XMVECTOR pos;	//光源の位置ベクトルまたは方向ベクトル
public:
	Light();
	~Light();
	void SetLight(XMVECTOR update);
	XMFLOAT4 GetLight();
};