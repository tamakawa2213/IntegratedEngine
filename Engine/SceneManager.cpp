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

		//‰æ–Ê‚Ì–¾‚é‚³‚ðŒ³‚É–ß‚·
		assFunc_.SetFadeout(1);

		//UI‚Ì•R•t‚¯‰ðœ
		RootUI* pRootUI = (RootUI*)FindObject("RootUI");
		pRootUI->UnLink();

		pScene_->Update();
	}
	
}
