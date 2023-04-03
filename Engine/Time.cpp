#include "Time.h"
#include <chrono>

//変数
namespace
{
	bool Lock_ = true;		//現在の状態で固定するか

	std::chrono::steady_clock::time_point Prev = std::chrono::steady_clock::now();	//前フレームのシステム時間を保存
	std::chrono::steady_clock::time_point Now = std::chrono::steady_clock::now();	//現在のシステム時間を保存
	std::chrono::milliseconds MilliSeconds_ = std::chrono::milliseconds::zero();	//経過時間
}

namespace Time
{
	void Update()
	{
		//2つのシステム時間を更新
		Prev = Now;
		Now = std::chrono::steady_clock::now();

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
}