#pragma once
#include <string>
#include "Transform.h"

class Texture;

namespace Image
{
	int Load(const std::string& filename);
	void SetTransform(int hPict, const Transform& transform);
	void SetPosition(int hPict, const XMFLOAT3& pos);

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