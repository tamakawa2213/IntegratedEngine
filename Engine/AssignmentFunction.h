#pragma once
#include <vector>

//�w�肵���������֐����Ăяo���N���X
class AssignmentFunction
{
	//int���L�[�Ƃ��Ă��̐��������t���[���Ăяo��
	std::vector<std::pair<int*, void(AssignmentFunction::*)()>> func_;

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