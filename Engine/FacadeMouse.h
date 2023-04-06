#pragma once
#include <vector>

//�}�E�X�̏����݂̂�ێ����鉼�z�N���X
class FacadeMouse
{
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionLeft_;
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionRight_;
	std::vector<std::pair<bool(*)(int), void(FacadeMouse::*)()>> FunctionWheel_;
public:
	FacadeMouse();
	~FacadeMouse() {}

	void FacadeMethod();	//���z�֐��̌Ăяo�����s��

	virtual void ClickLeftFirst() {}	//���N���b�N�������Ɉ�x�Ăяo��
	virtual void ClickLeft() {}			//���N���b�N���Ăяo��
	virtual void LeaveLeft() {}			//���N���b�N�����ꂽ�Ƃ��Ɉ�x�Ăяo��

	virtual void ClickRightFirst() {}	//�E�N���b�N�������Ɉ�x�Ăяo��
	virtual void ClickRight() {}		//�E�N���b�N���Ăяo��
	virtual void LeaveRight() {}		//�E�N���b�N�����ꂽ�Ƃ��Ɉ�x�Ăяo��

	virtual void ClickWheelFirst() {}	//�z�C�[���������Ɉ�x�Ăяo��
	virtual void ClickWheel() {}		//�z�C�[���N���b�N���Ăяo��
	virtual void LeaveWheel() {}		//�z�C�[���N���b�N�����ꂽ�Ƃ��Ɉ�x�Ăяo��
};