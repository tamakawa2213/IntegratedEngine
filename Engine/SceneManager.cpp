#include "SceneManager.h"
#include "Model.h"
#include "Image.h"
#include "RootUI.h"

SceneManager::SceneManager(GameObject* parent) : GameObject(parent, "SceneManager"), pScene_(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
}

void SceneManager::Update()
{
	if (pScene_ != nullptr && pScene_->CurrentSceneID_ != pScene_->NextSceneID_)
	{
		KillAllChildren();
		Model::Release();
		Image::Release();

		//画面の明るさを元に戻す
		assFunc_.SetFadeout(1);

		//UIの紐付け解除
		RootUI* pRootUI = (RootUI*)FindObject("RootUI");
		pRootUI->UnLink();

		pScene_->Update();
	}
	
}
