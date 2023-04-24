#pragma once
#include <DirectXMath.h>

struct RayCastData
{
	DirectX::XMFLOAT3 start;	//レイの始点
	DirectX::XMFLOAT3 dir;		//発射する方向
	float dist;					//始点と命中した位置の距離
	bool hit;					//当たったかどうか
	DirectX::XMFLOAT3 hitpos;	//命中した位置

	void CreateMouseRay();	//マウスのレイを作成する

	RayCastData() : start(), dir(), dist(9999.0f), hit(false), hitpos() {}
};