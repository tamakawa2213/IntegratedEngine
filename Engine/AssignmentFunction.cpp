#include "AssignmentFunction.h"
#include "Image.h"
#include "Model.h"
#include "Direct3D.h"

namespace
{
	int SettingFade = 0;	//�t�F�[�h�A�E�g���ݒ肳�ꂽ���̒l���i�[�����
	float ChangeRate = 0;	//��ʕ\���̕ω���
}

AssignmentFunction::AssignmentFunction()
	:Fadeout_(0)
{
	func_.clear();
	func_.push_back({ &Fadeout_, &AssignmentFunction::FadeOut });
}

AssignmentFunction::~AssignmentFunction()
{
	func_.clear();
}

void AssignmentFunction::Update()
{
	//for (auto itr = func_.begin(); itr != func_.end(); itr++)
	for (auto itr : func_)
	{
		//�C�e���[�^���w���Ă�ϐ���0���傫���ꍇ
		if (*itr.first > 0)
		{
			//�֘A����֐������s
			(this->*itr.second)();

			//�f�N�������g
			(*itr.first)--;
		}
	}
	
}

void AssignmentFunction::SetFadeout(int frame)
{
	SettingFade = frame;
	Fadeout_ = SettingFade;
	ChangeRate = 1.0f / frame;
}

void AssignmentFunction::FadeOut()
{
	//0�Ŋ���Ȃ��悤��
	if (SettingFade == 0)
		return;

	float alp = fabsf((float)Fadeout_ / SettingFade * 2 - 1);
	Image::AllAlterAlpha(alp);
	Model::AllAlterAlpha(alp);
	Direct3D::BackGroundColor[0] = 0;
	Direct3D::BackGroundColor[1] = alp / 2;
	Direct3D::BackGroundColor[2] = alp / 2;
	Direct3D::BackGroundColor[3] = 1.0f;
}