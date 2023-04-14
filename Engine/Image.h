#pragma once
#include <string>
#include "Transform.h"

class Texture;

namespace Image
{
	int Load(const std::string& filename);
	void SetTransform(int hPict, const Transform& transform);
	void SetPosition(int hPict, const XMFLOAT3& pos);

	//指定した画像番号の位置を取得
	XMFLOAT3 GetPosition(int hPict);
	std::string GetFileName(int hPict);

	void Draw(int hModel);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <param name="hModel">判定したいモデル番号</param>
	/// <returns>当たっていればtrue</returns>
	bool IsHitCursor(int hPict);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <returns>当たっていればその画像番号、当たっていなければ-1</returns>
	int IsHitCursorAny();

	void Release();

	//テクスチャのconstポインタを取得する
	const Texture* GetpTexture(int hPict);

	//現在ロードされている全ての画像の透明度を変更
	void AllAlterAlpha(float alpha);
}