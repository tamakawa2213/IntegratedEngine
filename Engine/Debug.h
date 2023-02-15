#pragma once
#if _DEBUG

//デバッグモード専用
enum class DEBUG_MODE
{
	IMAGE_POSITIONING = 0
};

namespace Debug
{
	//画像の位置調整
	void ImagePositioning();

	void BranchMode();

	void SetDebugMode(DEBUG_MODE mode);
};

#endif