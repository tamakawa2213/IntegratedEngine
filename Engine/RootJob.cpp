#include "RootJob.h"
#include "SceneManager.h"
#include "Debug.h"
#include "RootUI.h"
#include "PtrObserver.h"

RootJob::RootJob(GameObject* parent) : GameObject()
{
}

RootJob::RootJob(GameObject* parent, const std::string& name) : GameObject(parent, name)
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize(Scene* pScene)
{
	PtrObserver::SetRootUI(Instantiate<RootUI>(this));
	PtrObserver::SetSceneManager(Instantiate<SceneManager>(this));
	PtrObserver::GetSceneManager()->SetSceneObject(pScene);
}

void RootJob::DebugMode()
{
	Debug::BranchMode();
}
