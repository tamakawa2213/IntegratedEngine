#include "Time.h"
#include <chrono>

//�ϐ�
namespace
{
	bool Lock_ = true;		//���݂̏�ԂŌŒ肷�邩

	std::chrono::steady_clock::time_point Prev = std::chrono::steady_clock::now();	//�O�t���[���̃V�X�e�����Ԃ�ۑ�
	std::chrono::steady_clock::time_point Now = std::chrono::steady_clock::now();	//���݂̃V�X�e�����Ԃ�ۑ�
	std::chrono::milliseconds MilliSeconds_{ 0 };	//�o�ߎ���
}

namespace Time
{
	void Update()
	{
		//2�̃V�X�e�����Ԃ��X�V
		Prev = Now;
		Now = std::chrono::steady_clock::now();

		//Lock����Ă�����X�V���Ȃ�
		if (Lock_)
			return;

		//���݂ƒ��O�̍������Z
		MilliSeconds_ += std::chrono::duration_cast<std::chrono::milliseconds>(Now - Prev);
	}

	void Reset()
	{
		MilliSeconds_ = std::chrono::milliseconds::zero();
	}

	void Lock()
	{
		Lock_ = true;
	}

	void UnLock()
	{
		Lock_ = false;
	}

	int GetTimei()
	{
		return (int)std::chrono::duration_cast<std::chrono::seconds>(MilliSeconds_).count();
	}
	float GetTimef()
	{
		return (float)MilliSeconds_.count() / 1000;
	}
	double GetTime()
	{
		return (double)MilliSeconds_.count() / 1000;
	}
	int GetMinutes()
	{
		return (int)std::chrono::duration_cast<std::chrono::minutes>(MilliSeconds_).count();
	}
	int GetHours()
	{
		return (int)std::chrono::duration_cast<std::chrono::hours>(MilliSeconds_).count();
	}
}