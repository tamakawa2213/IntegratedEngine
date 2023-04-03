#pragma once
#include <DirectXMath.h>

//拡大率に関する演算を実装した構造体
struct Scale
{
	float x;
	float y;
	float z;

	//算術演算子のオーバーロード
	Scale operator *(const float i);
	Scale operator /(const float i);

	//代入演算子のオーバーロード
	Scale& operator =(const DirectX::XMFLOAT3 fl);
	Scale& operator =(const Scale& fl);
	Scale& operator =(const float i);

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

	Scale() : x(1), y(1), z(1) {}
	Scale(DirectX::XMFLOAT3 fl) : x(fl.x), y(fl.y), z(fl.z) {}
	Scale(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};