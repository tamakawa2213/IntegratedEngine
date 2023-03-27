#pragma once

#include <DirectXMath.h>
#include <dInput.h>
#include "XInput.h"

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

namespace Input
{
	extern XINPUT_STATE controllerState_;

	void Initialize(HWND hWnd);
	void Update();

	//�L�[�{�[�h
	namespace Keyboard
	{
		bool IsPush(int keyCode);		//�L�[�������Ă����
		bool Down(int keyCode);			//�L�[���������u��
		bool Up(int keyCode);			//�L�[�𗣂����u��
		bool AnyPush();					//�����ꂩ�̃L�[�������Ă����
		bool AnyDown();					//�����ꂩ�̃L�[���������u��
		bool AnyUp();					//�����ꂩ�̃L�[�𗣂����u��
		bool Add();						//�����Ă���L�[���O�����������
		bool Remove();					//�����Ă���L�[���O��������Ȃ���
	}

	//�}�E�X
	namespace Mouse
	{
		DirectX::XMFLOAT3 GetPosition();		//�}�E�X�̈ʒu���擾
		void SetPosition(int x, int y);			//�}�E�X�̈ʒu��ݒ�
		DirectX::XMFLOAT3 GetMovement();		//�}�E�X�̈ړ��ʂ��擾
		void SetWheel(int r);					//�}�E�X�z�C�[���̉�]�ʂ�ݒ�
		int GetWheel();							//�}�E�X�z�C�[���̉�]�ʂ��擾
		bool IsMove();							//�}�E�X����������
		bool IsPush(int mouseBotton);			//�}�E�X�������Ă����
		bool Down(int mouseBotton);				//�}�E�X���������u��
		bool Up(int mouseBotton);				//�}�E�X�𗣂����u��
	}

	//�R���g���[���[
	namespace Controller
	{
		bool IsPush(int ctrlBotton);				//�{�^���������Ă����
		bool Up(int ctrlBotton);					//�{�^���𗣂����u��
		bool Down(int ctrlBotton);					//�{�^�����������u��
		DirectX::XMFLOAT3 Inclination_StickL();		//L�X�e�B�b�N�̌X��
		DirectX::XMFLOAT3 Inclination_StickR();		//R�X�e�B�b�N�̌X��
		float Inclination_Trigger(BYTE Trigger);	//�g���K�[�̉������݋
	}

	void Release();
};