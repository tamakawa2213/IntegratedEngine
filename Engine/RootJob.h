#pragma once
#include "GameObject.h"

class Scene;

class RootJob : public GameObject
{
public:
	RootJob(GameObject* parent);
	RootJob(GameObject* parent, const std::string& name);
	~RootJob();

	void Initialize(Scene* pScene);	//èâä˙âª

	void DebugMode() override;
};