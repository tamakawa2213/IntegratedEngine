#include "RootUI.h"

RootUI::RootUI(GameObject* parent)
	: GameObject(parent, "RootUI"), pUI_(nullptr)
{
}

RootUI::~RootUI()
{
}

void RootUI::FixedUpdate()
{
	if (pUI_)
	{
		pUI_->Update();
	}
}

void RootUI::DrawUnique()
{
	if (pUI_)
	{
		pUI_->Draw();
	}
}

void RootUI::Release()
{
	if (pUI_)
	{
		pUI_->Release();
	}
}

void RootUI::UnLink()
{
	SAFE_RELEASE(pUI_);
}

void RootUI::SceneChange(SCENE_ID id)
{
	SCENE_CHANGE(id);
}