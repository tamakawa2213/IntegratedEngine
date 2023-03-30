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
		//イテレータが指してる変数が0より大きい場合
		if (*itr.first > 0)
		{
			//関連する関数を実行
			(this->*itr.second)();

			//デクリメント
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
	//0で割らないように
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