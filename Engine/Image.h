#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "Transform.h"

struct ImageSet
{
	Sprite* pSprite = nullptr;	//Spriteのポインタ
	Transform transform;		//transformクラス
	LPCWSTR FileName = L"";		//ファイルの名前
	bool FindFbx = false;		//Fbxファイルを事前にロードしているか
};

namespace Image
{
	int Load(LPCWSTR filename);
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void SetPosition(int hModel, XMFLOAT3 pos);

	//指定した画像番号の位置を取得
	XMFLOAT3 GetPosition(int hModel);

	void Draw(int hModel);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <param name="hModel">判定したいモデル番号</param>
	/// <returns>当たっていればtrue</returns>
	bool IsHitCursor(int hModel);

	/// <summary>画像とマウスカーソルが当たっているか</summary>
	/// <returns>当たっていればその画像番号、当たっていなければ-1</returns>
	int IsHitCursorAny();

	void Release();
}