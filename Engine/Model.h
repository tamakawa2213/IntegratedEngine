#pragma once
#include <string>
#include "Fbx.h"
#include "Light.h"
#include "Transform.h"

namespace Model
{
	int Load(const std::string& filename);
	void SetTransform(unsigned hModel, const Transform& transform);
	void Draw(unsigned hModel, const Light* lightpos = nullptr);
	void Draw(unsigned hModel, const Transform& transform, const Light* lightpos = nullptr);
	void Draw(unsigned hModel, const XMFLOAT3& Chroma, float Bright, const Light* lightpos = nullptr);

	void Release();

	void RayCast(unsigned hModel, RayCastData& Raydata);

	void SetTexture(unsigned hModel, const Texture* tex);

	//���݃��[�h����Ă���S�Ẵ��f���̓����x��ύX
	void AllAlterAlpha(float alpha);
}