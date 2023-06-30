#pragma once
#include <string>
#include "Transform.h"

class Texture;

namespace Image
{
	int Load(const std::string& filename);
	void SetTransform(unsigned hPict, const Transform& transform);
	void SetPosition(unsigned hPict, const XMFLOAT3& pos);

	void SetAlpha(unsigned hPict, float alpha);

	//指定した画像番号の位置を取得
	XMFLOAT3 GetPosition(unsigned hPict);
	std::string GetFileName(unsigned hPict);

	void Draw(unsigned hModel);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <param name="hModel">判定したいモデル番号</param>
	/// <returns>当たっていればtrue</returns>
	bool IsHitCursor(unsigned hPict);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <returns>当たっていればその画像番号、当たっていなければ-1</returns>
	int IsHitCursorAny();

	void Release();

	//テクスチャのconstポインタを取得する
	const Texture* GetpTexture(unsigned hPict);

	//現在ロードされている全ての画像の透明度を変更
	void AllAlterAlpha(float alpha);
}