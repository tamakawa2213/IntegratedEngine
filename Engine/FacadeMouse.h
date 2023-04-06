#pragma once
#include <vector>

//マウスの処理のみを保持する仮想クラス
class FacadeMouse
{
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionLeft_;
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionRight_;
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionWheel_;
public:
	FacadeMouse();
	~FacadeMouse() {}

	void FacadeMethod();	//仮想関数の呼び出しを行う

	virtual void ClickLeftFirst() {}	//左クリック押下時に一度呼び出し
	virtual void ClickLeft() {}			//左クリック中呼び出し
	virtual void LeaveLeft() {}			//左クリックが離れたときに一度呼び出し

	virtual void ClickRightFirst() {}	//右クリック押下時に一度呼び出し
	virtual void ClickRight() {}		//右クリック中呼び出し
	virtual void LeaveRight() {}		//右クリックが離れたときに一度呼び出し

	virtual void ClickWheelFirst() {}	//ホイール押下時に一度呼び出し
	virtual void ClickWheel() {}		//ホイールクリック中呼び出し
	virtual void LeaveWheel() {}		//ホイールクリックが離れたときに一度呼び出し
};