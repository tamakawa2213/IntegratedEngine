#include "Time.h"
#include <chrono>
#include <list>

//変数
namespace
{
	bool Lock_ = true;		//現在の状態で固定するか

	std::chrono::steady_clock::time_point Prev = std::chrono::steady_clock::now();	//前フレームのシステム時間を保存
	std::chrono::steady_clock::time_point Now = std::chrono::steady_clock::now();	//現在のシステム時間を保存
	std::chrono::milliseconds MilliSeconds_ = std::chrono::milliseconds::zero();	//経過時間

	std::list<Time::Watch*> WatchList = {};	//生成されたWatchクラスを管理しておく配列
}

namespace Time
{
	void Update()
	{
		//2つのシステム時間を更新
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

		//Lockされていたら更新しない
		if (Lock_)
			return;

		//現在と直前の差を加算
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
		//配列の中に存在する自身を検索し、削除する
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