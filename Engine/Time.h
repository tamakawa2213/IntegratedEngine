#pragma once

/// <summary>
/// DirectX環境に依存しないTime
/// </summary>
namespace Time
{
	//更新(Mainで呼び出し)
	void Update();

	//時間を初期化
	void Reset();

	//時間の更新を一時停止
	void Lock();

	//時間の更新を再開
	void UnLock();

	//呼び出し時点の時間を取得
	int GetTimei();
	float GetTimef();
	double GetTime();
	int GetMinutes();
	int GetHours();
};

