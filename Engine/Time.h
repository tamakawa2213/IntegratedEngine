#pragma once

/// <summary>
/// DirectX���Ɉˑ����Ȃ�Time
/// </summary>
namespace Time
{
	//�X�V(Main�ŌĂяo��)
	void Update();

	//���Ԃ�������
	void Reset();

	//���Ԃ̍X�V���ꎞ��~
	void Lock();

	//���Ԃ̍X�V���ĊJ
	void UnLock();

	//�Ăяo�����_�̎��Ԃ��擾
	int GetTimei();
	float GetTimef();
	double GetTime();
	int GetMinutes();
	int GetHours();
};

