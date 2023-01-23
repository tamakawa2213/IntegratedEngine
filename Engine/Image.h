#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "Transform.h"

struct ImageSet
{
	Sprite* pSprite = nullptr;	//Sprite�̃|�C���^
	Transform transform;		//transform�N���X
	LPCWSTR FileName = L"";		//�t�@�C���̖��O
	bool FindFbx = false;		//Fbx�t�@�C�������O�Ƀ��[�h���Ă��邩
};

namespace Image
{
	int Load(LPCWSTR filename);
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);

	/// <summary>�摜�ƃ}�E�X�J�[�\�����������Ă��邩</summary>
	/// <param name="hModel">���肵�������f���ԍ�</param>
	/// <returns>�������Ă����true</returns>
	bool IsHitCursor(int hModel);

	void Release();
}