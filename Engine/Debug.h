#pragma once
#if _DEBUG

//�f�o�b�O���[�h��p
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