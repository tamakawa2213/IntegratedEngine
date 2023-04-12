#pragma once
#include <memory>

class SceneManager;

enum class SCENE_ID : int;

//SceneManager�Ƃ̋��n����S�����z�N���X
class Scene
{
	friend SceneManager;
protected:
	SCENE_ID CurrentSceneID_;
	SCENE_ID NextSceneID_;
	SceneManager* ptr_;
public:
	Scene(SCENE_ID cur) : CurrentSceneID_(cur), NextSceneID_(cur), ptr_(nullptr) {}
	virtual ~Scene() {}

	virtual void Initialize() = 0;

	virtual void Update() = 0;
};

