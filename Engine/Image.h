#pragma once
#include <string>
#include "Transform.h"

class Texture;

namespace Image
{
	int Load(const std::string& filename);
	void SetTransform(unsigned hPict, const Transform& transform);
	void SetPosition(unsigned hPict, const XMFLOAT3& pos);

	void SetAlpha(unsigned hPict, float alpha);

	//�w�肵���摜�ԍ��̈ʒu���擾
	XMFLOAT3 GetPosition(unsigned hPict);
	std::string GetFileName(unsigned hPict);

	void Draw(unsigned hModel);

	/// <summary>�摜�ƃ}�E�X�J�[�\�����������Ă��邩</summary>
	/// <param name="hModel">���肵�������f���ԍ�</param>
	/// <returns>�������Ă����true</returns>
	bool IsHitCursor(unsigned hPict);

	/// <summary>�摜�ƃ}�E�X�J�[�\�����������Ă��邩</summary>
	/// <returns>�������Ă���΂��̉摜�ԍ��A�������Ă��Ȃ����-1</returns>
	int IsHitCursorAny();

	void Release();

	//�e�N�X�`����const�|�C���^���擾����
	const Texture* GetpTexture(unsigned hPict);

	//���݃��[�h����Ă���S�Ẳ摜�̓����x��ύX
	void AllAlterAlpha(float alpha);
}