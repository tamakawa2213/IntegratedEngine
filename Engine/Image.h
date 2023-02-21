#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "Transform.h"

struct ImageSet
{
	Sprite* pSprite = nullptr;	//Sprite�̃|�C���^
	Transform transform;		//transform�N���X
	std::string FileName = "";	//�t�@�C���̖��O
	bool FindFbx = false;		//Fbx�t�@�C�������O�Ƀ��[�h���Ă��邩
	float Alpha = 1.0f;			//�摜�̓����x
};

namespace Image
{
	int Load(std::string filename);
	void SetTransform(int hPict, Transform transform);
	void SetPosition(int hPict, XMFLOAT3 pos);

	//�w�肵���摜�ԍ��̈ʒu���擾
	XMFLOAT3 GetPosition(int hPict);
	std::string GetFileName(int hPict);

	void Draw(int hModel);

	/// <summary>�摜�ƃ}�E�X�J�[�\�����������Ă��邩</summary>
	/// <param name="hModel">���肵�������f���ԍ�</param>
	/// <returns>�������Ă����true</returns>
	bool IsHitCursor(int hPict);

	/// <summary>�摜�ƃ}�E�X�J�[�\�����������Ă��邩</summary>
	/// <returns>�������Ă���΂��̉摜�ԍ��A�������Ă��Ȃ����-1</returns>
	int IsHitCursorAny();

	void Release();

	//�e�N�X�`����const�|�C���^���擾����
	const Texture* GetpTexture(int hPict);

	//���݃��[�h����Ă���S�Ẳ摜�̓����x��ύX
	void AllAlterAlpha(float alpha);
}