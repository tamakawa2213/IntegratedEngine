#pragma once
#include <DirectXMath.h>

//拡大率に関する演算を実装したクラス
class Scale
{
public:
	Scale();
	~Scale();

	float x;
	float y;
	float z;

	//算術演算子のオーバーロード
	Scale operator *(const float i);
	Scale operator /(const float i);

	//代入演算子のオーバーロード
	Scale& operator =(const DirectX::XMFLOAT3 fl);
	Scale& operator =(const Scale& fl);

	//複合代入演算子のオーバーロード
	Scale operator *=(const float i);
	Scale operator /=(const float i);

	//比較演算子のオーバーロード
	bool operator ==(const DirectX::XMFLOAT3 fl) const;
	bool operator ==(const Scale& fl) const;
	bool operator !=(const DirectX::XMFLOAT3 fl) const;
	bool operator !=(const Scale& fl) const;

	//XMFLOAT3にキャスト可能にする
	operator DirectX::XMFLOAT3() const;
};