#pragma once
#if _DEBUG

//�f�o�b�O���[�h��p
enum class DEBUG_MODE
{
	IMAGE_POSITIONING = 0
};

namespace Debug
{
	//�摜�̈ʒu����
	void ImagePositioning();

	void BranchMode();

	void SetDebugMode(DEBUG_MODE mode);
};

#endif