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
	void Draw(int hModel);
	bool IsHitCursor(int hModel);

	void Release();
}