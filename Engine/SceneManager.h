#pragma once
#include "GameObject.h"
#include "Scene.h"
#include <memory>

class SceneManager : public GameObject
{
	Scene* pScene_;
public:
	SceneManager(GameObject* parent);
	~SceneManager();

	void SetSceneObject(Scene* ptr) { pScene_ = ptr; pScene_->Initialize(); }

	void Initialize() override;
	void Update() override;
	
	void ChangeScene(SCENE_ID ID) { pScene_->NextSceneID_ = ID; }
};