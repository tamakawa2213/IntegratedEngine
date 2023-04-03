#pragma once
#include <DirectXMath.h>

//位置に関する演算を実装した構造体
struct Position
{
	float x;
	float y;
	float z;

	//算術演算子のオーバーロード
	Position operator +(DirectX::XMFLOAT3 fl);
	Position operator +(Position fl);
	Position operator -(DirectX::XMFLOAT3 fl);
	Position operator -(Position fl);

	//代入演算子のオーバーロード
	Position& operator =(const DirectX::XMFLOAT3 fl);
	Position& operator =(const Position& fl);

	//複合代入演算子のオーバーロード
	Position& operator +=(DirectX::XMFLOAT3 fl);
	Position& operator +=(Position& fl);
	Position& operator -=(DirectX::XMFLOAT3 fl);
	Position& operator -=(Position& fl);

	//比較演算子のオーバーロード
	bool operator ==(const DirectX::XMFLOAT3 fl) const;
	bool operator ==(const Position& fl) const;
	bool operator !=(const DirectX::XMFLOAT3 fl) const;
	bool operator !=(const Position& fl) const;

	//XMFLOAT3にキャスト可能にする
	operator DirectX::XMFLOAT3() const;

	Position() : x(0), y(0), z(0) {}
	Position(DirectX::XMFLOAT3 fl) : x(fl.x), y(fl.y), z(fl.z) {}
	Position(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};