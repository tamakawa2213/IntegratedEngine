#pragma once
#include "GameObject.h"
#include "UserInterface.h"
#include "SceneManager.h"
#include <memory>

class RootUI : public GameObject
{
	std::unique_ptr<UserInterface> pUI_;

public:
	RootUI(GameObject* parent);
	~RootUI();

	void FixedUpdate() override;

	void DrawUnique() override;

	void Release() override;

	//UIのオブジェクトとの紐付けを行う
	template <class T>
	void Link()
	{
		std::unique_ptr<T> ui = std::make_unique<T>();
		pUI_ = std::move(ui);
		pUI_->Initialize();
	}

	//紐付けを解除する
	void UnLink();

	void SceneChange(SCENE_ID id);
};