#include "MouseOperationUI.h"

MouseOperationUI::MouseOperationUI(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
}

MouseOperationUI::~MouseOperationUI()
{
}

void MouseOperationUI::FixedUpdate()
{
	FacadeMethod();
}
