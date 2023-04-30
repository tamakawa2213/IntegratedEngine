#include "PtrObserver.h"

namespace
{
	SceneManager* pSceneManager = nullptr;
	RootUI* pRootUI = nullptr;
}

namespace PtrObserver
{
	void SetSceneManager(SceneManager* ptr)
	{
		//一度紐付けたら書き換え不可にする
		if(pSceneManager == nullptr)
			pSceneManager = ptr;
	}
	SceneManager* GetSceneManager()
	{
		return pSceneManager;
	}
	void SetRootUI(RootUI* ptr)
	{
		//一度紐付けたら書き換え不可にする
		if (pRootUI == nullptr)
			pRootUI = ptr;
	}
	RootUI* GetRootUI()
	{
		return pRootUI;
	}
};