#pragma once
#include <string>
#include "Fbx.h"
#include "Light.h"
#include "Transform.h"

namespace Model
{
	int Load(const std::string& filename);
	void SetTransform(int hModel, const Transform& transform);
	void Draw(int hModel, const Light* lightpos = nullptr);
	void Draw(int hModel, const Transform& transform, const Light* lightpos = nullptr);
	void Draw(int hModel, const XMFLOAT3& Chroma, float Bright, const Light* lightpos = nullptr);

	void Release();

	void RayCast(int hModel, RayCastData& Raydata);

	void SetTexture(int hModel, const Texture* tex);

	//現在ロードされている全てのモデルの透明度を変更
	void AllAlterAlpha(float alpha);
}