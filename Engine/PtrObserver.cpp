#include "PtrObserver.h"

namespace
{
	SceneManager* pSceneManager;
	RootUI* pRootUI;
}

namespace PtrObserver
{
	void SetSceneManager(SceneManager* ptr)
	{
		pSceneManager = ptr;
	}
	SceneManager* GetSceneManager()
	{
		return pSceneManager;
	}
	void SetRootUI(RootUI* ptr)
	{
		pRootUI = ptr;
	}
	RootUI* GetRootUI()
	{
		return pRootUI;
	}
};