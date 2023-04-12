#pragma once
#include "SceneManager.h"
#include "RootUI.h"
#include <memory>


namespace PtrObserver
{
	void SetSceneManager(SceneManager* ptr);
	SceneManager* GetSceneManager();

	void SetRootUI(RootUI* ptr);
	RootUI* GetRootUI();

};