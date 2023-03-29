#pragma once
#include <DirectXMath.h>

//位置に関する演算を実装したクラス
class Position
{
public:
	Position();
	~Position();

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
	Position& operator -=(Position fl);

	//比較演算子のオーバーロード
	bool operator ==(const DirectX::XMFLOAT3 fl) const;
	bool operator ==(const Position& fl) const;
	bool operator !=(DirectX::XMFLOAT3 fl) const;
	bool operator !=(Position fl) const;

	//XMFLOAT3にキャスト可能にする
	operator DirectX::XMFLOAT3() const;
};