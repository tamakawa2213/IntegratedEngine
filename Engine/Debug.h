#pragma once

//�f�o�b�O���[�h��p
enum class DEBUG_MODE
{
	IMAGE_POSITIONING = 0,
	TEXT_WRITING
};

namespace Debug
{
	extern bool CallDebug_;

	//RootJob����Ăяo��
	void BranchMode();

	//�f�o�b�O���[�h�̕ύX
	void SetDebugMode(DEBUG_MODE mode);
};