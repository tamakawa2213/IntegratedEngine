#pragma once
#include "FacadeMouse.h"
#include "GameObject.h"

class MouseOperationUI : public GameObject, FacadeMouse
{
public:
	MouseOperationUI(GameObject* parent, const std::string& name);
	~MouseOperationUI();

	void FixedUpdate() override;
};

