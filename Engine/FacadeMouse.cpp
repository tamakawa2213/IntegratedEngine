#include "FacadeMouse.h"
#include "Input.h"

FacadeMouse::FacadeMouse()
{
	FunctionLeft_.clear();
	FunctionRight_.clear();
	FunctionWheel_.clear();

	FunctionLeft_.push_back({ &Input::Mouse::Down, &FacadeMouse::ClickLeftFirst });
	FunctionLeft_.push_back({ &Input::Mouse::IsPush, &FacadeMouse::ClickLeft });
	FunctionLeft_.push_back({ &Input::Mouse::Up, &FacadeMouse::LeaveLeft });

	FunctionRight_.push_back({ &Input::Mouse::Down, &FacadeMouse::ClickRightFirst });
	FunctionRight_.push_back({ &Input::Mouse::IsPush, &FacadeMouse::ClickRight });
	FunctionRight_.push_back({ &Input::Mouse::Up, &FacadeMouse::LeaveRight });

	FunctionWheel_.push_back({ &Input::Mouse::Down, &FacadeMouse::ClickWheelFirst });
	FunctionWheel_.push_back({ &Input::Mouse::IsPush, &FacadeMouse::ClickWheel });
	FunctionWheel_.push_back({ &Input::Mouse::Up, &FacadeMouse::LeaveWheel });
}

void FacadeMouse::FacadeMethod()
{
	//それぞれの判定が真ならば対応するメソッドを呼び出す
	for (auto itr : FunctionLeft_)
	{
		if (itr.first(0))
			(this->*itr.second)();
	}

	for (auto itr : FunctionRight_)
	{
		if (itr.first(1))
			(this->*itr.second)();
	}

	for (auto itr : FunctionWheel_)
	{
		if (itr.first(2))
			(this->*itr.second)();
	}
}