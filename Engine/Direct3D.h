#pragma once
//#ifndef _DIRECT3D_H_
//#define _DIRECT3D_H_

#include <assert.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum class SHADER_TYPE {
	Dimension2 = 0,	//2D�p�V�F�[�_�[
	Dimension3,		//3D�p�V�F�[�_�[
	POINT,			//�_�����V�F�[�_�[
	FOG,			//�t�H�O(��)
	BILLBOARD,
	MAX				//����͎g��Ȃ�
};

enum VP_TYPE
{
	VP_FULL,	//�S��ʕ`��
	VP_LEFT,	//��ʍ��`��
	VP_RIGHT,	//��ʉE�`��
	VP_MAX
};

enum class SCREEN_MODE
{
	FULL,		//��ʕ����Ȃ�
	SPLIT_2,		//����
	MAX
};

enum BLEND_MODE	//�u�����h���[�h�̎w��
{
	BLEND_DEFAULT,
	BLEND_ADD,
	BLEND_MAX
};

namespace Direct3D
{
	//extern = �ǂ����Ő�������Ă��āA�����񐶐�����Ȃ��悤�ɂ��邽�߂̂���
	extern ID3D11Device* pDevice;			//�f�o�C�X
	extern ID3D11DeviceContext* pContext;	//�f�o�C�X�R���e�L�X�g
	extern IDXGISwapChain* pSwapChain;		//�X���b�v�`�F�C��
	extern int scrWidth, scrHeight;			//�X�N���[���̕��ƍ���
	extern SCREEN_MODE SplitScrMode;
	extern float BackGroundColor[4];

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	

	void SetShader(SHADER_TYPE type);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	void SetViewPort(char lr);

	void SetBlendMode(char blendMode);
};

//#endif

