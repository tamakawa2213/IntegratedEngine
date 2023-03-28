#pragma once
#include <string>
#include <vector>
#include "Fbx.h"
#include "Light.h"
#include "Transform.h"

struct Fileset
{
	Fbx* pFbx = nullptr;		//Fbxのポインタ
	Transform transform;		//transformクラス
	std::string FileName;		//ファイルの名前
	bool FindFbx = false;		//Fbxファイルを事前にロードしているか
	float Alpha = 1.0f;			//モデルの透明度
};

namespace Model
{
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel, const Light* lightpos = nullptr, SHADER_TYPE type = SHADER_TYPE::Dimension3);
	void Draw(int hModel, Transform transform, const Light* lightpos = nullptr, SHADER_TYPE type = SHADER_TYPE::Dimension3);
	void Draw(int hModel, XMFLOAT3 Chroma, float Bright, const Light* lightpos = nullptr, SHADER_TYPE type = SHADER_TYPE::Dimension3);

	void Release();

	void RayCast(int hModel, RayCastData& Raydata);

	void SetTexture(int hModel, const Texture* tex);

	//現在ロードされている全てのモデルの透明度を変更
	void AllAlterAlpha(float alpha);
}