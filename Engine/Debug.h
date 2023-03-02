#pragma once

//デバッグモード専用
enum class DEBUG_MODE
{
	IMAGE_POSITIONING = 0,
	TEXT_WRITING
};

namespace Debug
{
	extern bool CallDebug_;

	//RootJobから呼び出し
	void BranchMode();

	//デバッグモードの変更
	void SetDebugMode(DEBUG_MODE mode);
};