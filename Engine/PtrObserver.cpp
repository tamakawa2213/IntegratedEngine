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
		//��x�R�t�����珑�������s�ɂ���
		if(pSceneManager == nullptr)
			pSceneManager = ptr;
	}
	SceneManager* GetSceneManager()
	{
		return pSceneManager;
	}
	void SetRootUI(RootUI* ptr)
	{
		//��x�R�t�����珑�������s�ɂ���
		if (pRootUI == nullptr)
			pRootUI = ptr;
	}
	RootUI* GetRootUI()
	{
		return pRootUI;
	}
};