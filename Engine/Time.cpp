#include "Time.h"
#include <chrono>
#include <list>

//�ϐ�
namespace
{
	bool Lock_ = true;		//���݂̏�ԂŌŒ肷�邩

	std::chrono::steady_clock::time_point Prev = std::chrono::steady_clock::now();	//�O�t���[���̃V�X�e�����Ԃ�ۑ�
	std::chrono::steady_clock::time_point Now = std::chrono::steady_clock::now();	//���݂̃V�X�e�����Ԃ�ۑ�
	std::chrono::milliseconds MilliSeconds_ = std::chrono::milliseconds::zero();	//�o�ߎ���

	std::list<Time::Watch*> WatchList = {};	//�������ꂽWatch�N���X���Ǘ����Ă����z��
}

namespace Time
{
	void Update()
	{
		//2�̃V�X�e�����Ԃ��X�V
		Prev = Now;
		Now = std::chrono::steady_clock::now();

		if(!WatchList.empty())
		for (auto&& itr : WatchList)
		{
			if (!itr->IsLock())
			{
				if(!itr->IsCountDown())
					itr->MilliSeconds_ += (int)std::chrono::duration_cast<std::chrono::milliseconds>(Now - Prev).count();
				else
					itr->MilliSeconds_ -= (int)std::chrono::duration_cast<std::chrono::milliseconds>(Now - Prev).count();
			}
		}

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

	int GetMilliSeconds()
	{
		return (int)MilliSeconds_.count();
	}

	int GetMinutes()
	{
		return (int)std::chrono::duration_cast<std::chrono::minutes>(MilliSeconds_).count();
	}
	int GetHours()
	{
		return (int)std::chrono::duration_cast<std::chrono::hours>(MilliSeconds_).count();
	}
	

	Watch::Watch() : Lock_(false), CountDown_(false), MilliSeconds_(0)
	{
		WatchList.push_back(this);
	}

	Watch::Watch(int sec) : Lock_(false), CountDown_(true), MilliSeconds_((int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(sec)).count())
	{
		WatchList.push_back(this);
	}

	Watch::~Watch()
	{
		//�z��̒��ɑ��݂��鎩�g���������A�폜����
		if(auto itr = std::find(WatchList.begin(), WatchList.end(), this); itr != WatchList.end())
			WatchList.erase(itr);
	}

	void Watch::Reset()
	{
		MilliSeconds_ = 0;
	}

	void Watch::Lock()
	{
		Lock_ = true;
	}

	void Watch::UnLock()
	{
		Lock_ = false;
	}

	int Watch::GetMilliSeconds()
	{
		return MilliSeconds_;
	}

	int Watch::GetMinutes()
	{
		return (int)std::chrono::duration_cast<std::chrono::minutes>(std::chrono::milliseconds(MilliSeconds_)).count();
	}

	int Watch::GetHours()
	{
		return (int)std::chrono::duration_cast<std::chrono::hours>(std::chrono::milliseconds(MilliSeconds_)).count();
	}

	bool Watch::IsCountDown()
	{
		return CountDown_;
	}
}