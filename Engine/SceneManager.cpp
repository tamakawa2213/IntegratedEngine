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

		//��ʂ̖��邳�����ɖ߂�
		assFunc_.SetFadeout(1);

		//UI�̕R�t������
		RootUI* pRootUI = (RootUI*)FindObject("RootUI");
		pRootUI->UnLink();

		pScene_->Update();
	}
	
}
