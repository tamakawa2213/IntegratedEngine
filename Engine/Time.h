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
	int GetMilliSeconds();

	template<typename T = int>
	T GetSeconds()
	{
		return (T)GetMilliSeconds() / 1'000;
	}
	
	int GetMinutes();
	int GetHours();
};

