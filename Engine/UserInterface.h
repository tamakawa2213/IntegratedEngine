#pragma once

class RootUI;

class UserInterface
{
protected:
	RootUI* pRootUI_;

public:
	UserInterface(RootUI* ui) : pRootUI_(ui) {}
	virtual ~UserInterface() {}

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Release() {}
};