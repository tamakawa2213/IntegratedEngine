#pragma once
#if _DEBUG

//デバッグモード専用
enum class DEBUG_MODE
{
	IMAGE_POSITIONING = 0,
	TEXT_WRITING
};

namespace Debug
{
	void BranchMode();

	void SetDebugMode(DEBUG_MODE mode);
};

#endif