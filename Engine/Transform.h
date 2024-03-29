#pragma once
#include "Position.h"
#include "Scale.h"

using namespace DirectX;

class GameObject;

//位置、向き、拡大率などを管理するクラス
class Transform
{
	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;		//拡大行列

public:
	Position position_;	//位置
	XMFLOAT3 rotate_;	//向き
	Scale scale_;		//拡大率
	Transform* pParent_;

	//コンストラクタ
	Transform();

	//デストラクタ
	~Transform();

	//各行列の計算
	void Calclation();

	//ワールド行列を取得
	XMMATRIX GetWorldMatrix();

	//法線変形用行列を取得
	XMMATRIX GetNormalMatrix();

	//指定した座標に向けて回転させる
	void LookAt(const XMFLOAT3& target);
};