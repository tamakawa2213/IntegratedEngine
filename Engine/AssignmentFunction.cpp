#include "AssignmentFunction.h"
#include "Image.h"
#include "Model.h"
#include "Direct3D.h"

namespace
{
	int SettingFade = 0;	//フェードアウトが設定された時の値が格納される
	float ChangeRate = 0;	//画面表示の変化率
}

AssignmentFunction::AssignmentFunction()
	:Fadeout_(3)
{
	func_.clear();
	func_.insert({ Fadeout_, &AssignmentFunction::FadeOut });
}

AssignmentFunction::~AssignmentFunction()
{
}

void AssignmentFunction::Update()
{
	for (auto itr = func_.begin(); itr != func_.end(); itr++)
	{
		if (itr->first > 0)
		{
			itr->second;
			//itr->first--;
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
	float alp = fabsf((0.5f - Fadeout_ / SettingFade * ChangeRate) * 2);
	Image::AllAlterAlpha(alp);
	Model::AllAlterAlpha(alp);
	Direct3D::BackGroundColor[0] = alp;
	Direct3D::BackGroundColor[1] = alp;
	Direct3D::BackGroundColor[2] = alp;
	Direct3D::BackGroundColor[3] = 1.0f;
}