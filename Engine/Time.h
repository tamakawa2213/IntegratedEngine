#pragma once

//�X�g�b�v�E�H�b�`�̂悤�ȋ@�\�������O���
namespace Time
{
	//Time�Ɠ����悤�ɌʂŎg�p�\�ȃN���X
	class Watch
	{
		bool Lock_;		//���݂̏�ԂŌŒ肷�邩
		bool CountDown_;	//�J�E���g�_�E�����J�E���g�A�b�v��

	public:
		int MilliSeconds_;	//�o�ߎ���

		Watch();
		Watch(int sec);
		~Watch();

		//���Ԃ�������
		void Reset();

		//���Ԃ̍X�V���ꎞ��~
		void Lock();

		//���Ԃ̍X�V���ĊJ
		void UnLock();

		//�Ăяo�����_�̎��Ԃ��擾
		int GetMilliSeconds() const;

		template<typename T = int>
		T GetSeconds() const
		{
			return (T)GetMilliSeconds() / 1'000;
		}

		int GetMinutes() const;
		int GetHours() const;

		//�J�E���g�_�E����Ԃ��ǂ���
		bool IsCountDown() const;
		bool IsLock() const { return Lock_; }
	};	//class Watch

	//������
	void Initialize();

	//�X�V(Main�ŌĂяo��)
	void Update();

	//���Ԃ�������
	void Reset();

	//���Ԃ̍X�V���ꎞ��~
	void Lock();

	//���Ԃ̍X�V���ĊJ
	void UnLock();

	//�Ăяo�����_�̎��Ԃ��擾
	int GetMilliSeconds();

	template<typename T = int>
	T GetSeconds()
	{
		return (T)GetMilliSeconds() / 1'000;
	}
	
	int GetMinutes();
	int GetHours();
};

