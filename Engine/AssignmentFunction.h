#pragma once
#include <unordered_map>
#include <functional>
#include <map>

//�w�肵���������֐����Ăяo���N���X
class AssignmentFunction
{
	//int���L�[�Ƃ��Ă��̐��������t���[���Ăяo��
	//std::multimap<void(AssignmentFunction::*)(), int> func_;
	std::unordered_multimap<int, void(AssignmentFunction::*)()> func_;
	//std::map<std::function<void()>, int> func_;

	int Fadeout_;
	void FadeOut();

public:
	//�R���X�g���N�^
	AssignmentFunction();

	//�f�X�g���N�^
	~AssignmentFunction();

	//GameObject�N���X�ŌĂяo��
	void Update();

	//�t�F�[�h�A�E�g�ɂ�����t���[����������
	void SetFadeout(int frame);
};