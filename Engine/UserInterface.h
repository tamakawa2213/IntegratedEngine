#pragma once

struct UserInterface
{
	UserInterface() {}
	virtual ~UserInterface() {}

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Release() {}
};