#pragma once

/// <summary>
/// DirectX環境に依存しないTime
/// </summary>
namespace Time
{
	//初期化(Mainで初期化)
	void Initialize(int FPS);

	//更新(Mainで呼び出し)
	void Update();

	void Reset();

	void Lock();

	void UnLock();

	//呼び出し時点の時間を取得
	int GetTimei();
	float GetTimef();
	double GetTimew();
};

