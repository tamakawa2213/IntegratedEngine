#pragma once

class SceneManager;

enum class SCENE_ID;

//SceneManagerとの橋渡しを担う仮想クラス
class Scene
{
	friend SceneManager;
protected:
	SCENE_ID CurrentSceneID_;
	SCENE_ID NextSceneID_;
public:
	Scene() : CurrentSceneID_(), NextSceneID_() {}
	virtual ~Scene() {}

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	void SceneChange(SCENE_ID id) { NextSceneID_ = id; }

};

