#include "Time.h"
#include <chrono>
#include <list>

//�ϐ�
namespace
{
	bool Lock_ = true;		//���݂̏�ԂŌŒ肷�邩

	std::chrono::steady_clock::time_point Prev = std::chrono::steady_clock::now();	//�O�t���[���̃V�X�e�����Ԃ�ۑ�
	std::chrono::steady_clock::time_point Now = std::chrono::steady_clock::now();	//���݂̃V�X�e�����Ԃ�ۑ�

	std::list<Time::Watch*> WatchList = {};	//�������ꂽWatch�N���X���Ǘ����Ă����z��

	std::unique_ptr<Time::Watch> WorldTime;
}

namespace Time
{
	void Initialize()
	{
		WorldTime = std::make_unique<Time::Watch>();
	}

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
	}

	void Reset()
	{
		WorldTime->Reset();
	}

	void Lock()
	{
		WorldTime->Lock();
	}

	void UnLock()
	{
		WorldTime->UnLock();
	}

	int GetMilliSeconds()
	{
		return WorldTime->GetMilliSeconds();
	}

	int GetMinutes()
	{
		return WorldTime->GetMinutes();
	}
	int GetHours()
	{
		return WorldTime->GetHours();
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

	int Watch::GetMilliSeconds() const
	{
		return MilliSeconds_;
	}

	int Watch::GetMinutes() const
	{
		return (int)std::chrono::duration_cast<std::chrono::minutes>(std::chrono::milliseconds(MilliSeconds_)).count();
	}

	int Watch::GetHours() const
	{
		return (int)std::chrono::duration_cast<std::chrono::hours>(std::chrono::milliseconds(MilliSeconds_)).count();
	}

	bool Watch::IsCountDown() const
	{
		return CountDown_;
	}
}