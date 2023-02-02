#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Transform;

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);

	//線分とポリゴンの当たり判定を行う関数
	//第一引数 : 線分の始点　第二引数 : 線分の終点　第三～第五引数 : ポリゴンの各頂点
	bool SegmentToPlane(XMFLOAT3 segstart, XMFLOAT3 segend, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);

	//ベクトルとポリゴンの当たり判定を行う関数
	//第一引数 : ベクトルの始点　第二引数 : ベクトルの向き　第三～第五引数 : ポリゴンの各頂点
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);

	//距離も返す当たり判定
	bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, float* dist);

	/// <summary>ウェーブレット関数を生成して指定した位置の値を返す関数</summary>
	/// <param name="Incli">傾き</param>
	/// <param name="Coord">取得したい値の座標</param>
	/// <returns>指定した値の座標</returns>
	float Wavelet(float Incli, float Coord);

	/// <summary>ウェーブレット関数を生成して指定した2次元座標を返す関数</summary>
	/// <param name="Incli_X">X方向の傾き</param>
	/// <param name="Incli_Y">Y方向の傾き</param>
	/// <param name="Coord_X">取得したい値のX座標</param>
	/// <param name="Coord_Y">取得したい値のY座標</param>
	/// <returns>指定した値の高さ</returns>
	float Waveret_2Dim(float Incli_X, float Incli_Y, float Coord_X, float Coord_Y);

	/// <summary>xorshift疑似乱数</summary>
	/// <param name="v">適当な値</param>
	/// <returns>ランダムなハッシュ値</returns>
	int xorshift32(int v);

	//整数を取得する関数
	//第一引数 : 対象の値 第二引数 : 求める最小の桁 第三引数 : 求める最大の桁
	//注意 : 一桁目は0, 二桁目は1というように指定しなければならない
	//戻り値 : 切り抜いた値
	int GetDigits(int value, int m, int n);

	//小数点以下の指定した桁を整数値で取得する関数
	int GetFraction(float value, int m);

	/// <summary>素数を生成させる関数</summary>
	/// <param name="article">生成させる個数</param>
	/// <returns>生成された素数のvector</returns>
	std::vector<int> MakePrime(int article);

	/// <summary>2点間の距離を返す関数</summary>
	/// <returns>距離</returns>
	float GetDistance(Transform tr1, Transform tr2);
	float GetDistance(XMFLOAT3 tr1, XMFLOAT3 tr2);

	/// <summary>球同士の距離を返す</summary>
	/// <param name="tr1">自身の3次元座標</param>
	/// <param name="tr2">対象の3次元座標</param>
	/// <param name="radius1">自身の半径</param>
	/// <param name="radius2">対象の半径</param>
	/// <returns>距離(衝突していれば0が返る)</returns>
	float GetDistanceSphere(XMFLOAT3 tr1, XMFLOAT3 tr2, float radius1, float radius2);

	/// <summary>x = 0でy = 1,x = 1でy = 0になる三次関数</summary>
	/// <param name="Coord">取得したいxの位置</param>
	/// <returns>取得したyの値</returns>
	float Func_Cubic(float Coord);

	/// <summary>原点を通る任意軸回転</summary>
	/// <param name="pos">移動させたいものの位置</param>
	/// <param name="rot">回転角度(度数法)</param>
	/// <param name="axis">原点以外の軸となる位置ベクトル</param>
	/// <param name="end">軸のもう一端の位置ベクトル(無指定で原点になる)</param>
	void ArbRotationAxis(XMFLOAT3* pos, float rot, XMVECTOR axis, XMVECTOR end = {0,0,0,0});

	/// <summary>原点を通る任意軸回転</summary>
	/// <param name="pos">移動させたいものの位置</param>
	/// <param name="rad">回転角度(ラジアン)</param>
	/// <param name="axis">原点以外の軸となる位置ベクトル</param>
	/// <param name="end">軸のもう一端の位置ベクトル(無指定で原点になる)</param>
	void ArbRotationAxisR(XMFLOAT3* pos, float rad, XMVECTOR axis, XMVECTOR end = { 0,0,0,0 });

	/// <summary>値を0~指定した値に合わせる関数</summary>
	/// <param name="axis">調整したい値のポインタ</param>
	/// <param name="value">調整したい値の最大値</param>
	void FixValue(float* axis, int value);

	//画像のTransformの位置をピクセルに変換
	XMFLOAT3 TransformToPixel(XMFLOAT3 transform);
	//画像のピクセルをTransformに変換
	XMFLOAT3 PixelToTransform(XMFLOAT3 pixel);
};