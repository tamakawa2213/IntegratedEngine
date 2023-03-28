#pragma once
#include <string>
#include <vector>
#include "Fbx.h"
#include "Light.h"
#include "Transform.h"

struct Fileset
{
	Fbx* pFbx = nullptr;		//Fbx�̃|�C���^
	Transform transform;		//transform�N���X
	std::string FileName;		//�t�@�C���̖��O
	bool FindFbx = false;		//Fbx�t�@�C�������O�Ƀ��[�h���Ă��邩
	float Alpha = 1.0f;			//���f���̓����x
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

	//���݃��[�h����Ă���S�Ẵ��f���̓����x��ύX
	void AllAlterAlpha(float alpha);
}