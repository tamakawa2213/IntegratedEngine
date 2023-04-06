#include "Direct3D.h"
#include "Camera.h"
#include "CallDef.h"
#include <d3dcompiler.h>
#include <iterator>

//�ϐ�
namespace Direct3D
{
	//Direct3D�֘A
	ID3D11Device* pDevice = nullptr;						//�f�o�C�X
	ID3D11DeviceContext* pContext = nullptr;				//�f�o�C�X�R���e�L�X�g
	IDXGISwapChain* pSwapChain = nullptr;					//�X���b�v�`�F�C��
	ID3D11RenderTargetView* pRenderTargetView = nullptr;	//�����_�[�^�[�Q�b�g�r���[
	ID3D11BlendState* pBlendState[BLEND_MAX];				//�u�����h�X�e�[�g
	ID3D11Texture2D* pDepthStencil = nullptr;				//�[�x�X�e���V��
	ID3D11DepthStencilView* pDepthStencilView = nullptr;	//�[�x�X�e���V���r���[
	ID3D11DepthStencilState* pDepthStencilState[BLEND_MAX];

	//�V�F�[�_�֘A
	struct SHADER {
		ID3D11VertexShader* pVertexShader = nullptr;		//���_�V�F�[�_�[
		ID3D11PixelShader* pPixelShader = nullptr;			//�s�N�Z���V�F�[�_�[
		ID3D11InputLayout* pVertexLayout = nullptr;			//���_�C���v�b�g���C�A�E�g
		ID3D11RasterizerState* pRasterizerState = nullptr;	//���X�^���C�U�[
	};
	SHADER shader[(int)SHADER_TYPE::MAX];

	int scrWidth, scrHeight;

	char SplitScrMode = SCREEN_FULL;

	//�w�i�F
	float BackGroundColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	D3D11_VIEWPORT vp[VP_MAX];
}

namespace Direct3D
{
	//�V�F�[�_�[�̏�����
	HRESULT InitShader2D();
	HRESULT InitShader3D();
	HRESULT InitShaderPoint();
	HRESULT InitShaderFog();
	HRESULT InitShaderBillboard();
	HRESULT InitPixelShader(LPCWSTR filename, SHADER_TYPE type);	//�s�N�Z���V�F�[�_�̍쐬
}

//������
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	scrWidth = winW;
	scrHeight = winH;

	HRESULT hr;

	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW;		//��ʕ�
	scDesc.BufferDesc.Height = winH;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V

	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,						// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,								// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,						// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,								// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,				// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,						// ��ł��낢��ݒ肵���\����
		&pSwapChain,					// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice,						// ������������Device�A�h���X���Ԃ��Ă���
		&level,							// ������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext);						// ������������Context�̃A�h���X���Ԃ��Ă���
	HR_FAILED(hr, L"�f�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C���̍쐬�Ɏ��s���܂���");

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	assert(pBackBuffer != nullptr);

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	HR_FAILED_RELEASE(hr, L"�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���", pBackBuffer);
	
	//�����_�����O���ʂ�\������͈�
	vp[VP_FULL].Width = (float)winW;	//��
	vp[VP_FULL].Height = (float)winH;	//����
	vp[VP_FULL].MinDepth = 0.0f;		//��O
	vp[VP_FULL].MaxDepth = 1.0f;		//��
	vp[VP_FULL].TopLeftX = 0;			//��
	vp[VP_FULL].TopLeftY = 0;			//��

	vp[VP_LEFT].Width = (float)winW / 2;//��
	vp[VP_LEFT].Height = (float)winH;	//����
	vp[VP_LEFT].MinDepth = 0.0f;		//��O
	vp[VP_LEFT].MaxDepth = 1.0f;		//��
	vp[VP_LEFT].TopLeftX = 0;			//��
	vp[VP_LEFT].TopLeftY = 0;			//��

	vp[VP_RIGHT].Width = (float)winW / 2;	//��
	vp[VP_RIGHT].Height = (float)winH;		//����
	vp[VP_RIGHT].MinDepth = 0.0f;			//��O
	vp[VP_RIGHT].MaxDepth = 1.0f;			//��
	vp[VP_RIGHT].TopLeftX = (float)winW / 2;//��
	vp[VP_RIGHT].TopLeftY = 0;				//��

	//�[�x�X�e���V���r���[�̍쐬
	{
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = winW;
		descDepth.Height = winH;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
		assert(pDepthStencil != 0);
		pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);
	}

	//�[�x�e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬
	{
		//�f�t�H���g
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = true;
		pDevice->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_DEFAULT]);
		pContext->OMSetDepthStencilState(pDepthStencilState[BLEND_DEFAULT], 0);

		//���Z�����p�i�������݂Ȃ��j
		desc.StencilEnable = false;
		pDevice->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_ADD]);
	}

	//�u�����h�X�e�[�g�̍쐬
	{
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pDevice->CreateBlendState(&BlendDesc, &pBlendState[BLEND_DEFAULT]);
		float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		pContext->OMSetBlendState(pBlendState[BLEND_DEFAULT], blendFactor, UINT_MAX);

		//���Z�����i�d�Ȃ�قǌ����Č�������ʁj
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pDevice->CreateBlendState(&BlendDesc, &pBlendState[BLEND_ADD]);
	}

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// �f�[�^�̓��͎�ނ��w��
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);		// �`����ݒ�
	
	

	//�V�F�[�_�[����(�����������Ȃ����̂ŕ���)
	hr = InitShader();
	if (FAILED(hr)) {//���s������Ahr(����)��return����
		return hr;
	}
	return S_OK;	//HRESULT�^�̖߂�l��Ԃ��K�v������
}

//�V�F�[�_�[����
HRESULT Direct3D::InitShader()
{
	HRESULT hr;

	hr = InitShader2D();
	assert(hr == S_OK);

	hr = InitShader3D();
	assert(hr == S_OK);

	hr = InitShaderPoint();
	assert(hr == S_OK);

	hr = InitShaderFog();
	assert(hr == S_OK);

	hr = InitShaderBillboard();
	assert(hr == S_OK);

	return S_OK;
}

HRESULT Direct3D::InitPixelShader(LPCWSTR filename, SHADER_TYPE type)
{
	HRESULT hr;

	// �s�N�Z���V�F�[�_(PixelShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(filename, nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shader[(int)type].pPixelShader);

	HR_FAILED_RELEASE(hr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", pCompilePS);

	SAFE_RELEASE(pCompilePS);		//�R���p�C�������s�N�Z���V�F�[�_�����

	return hr;
}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	///////////////////////////////////////////////////// 2D�p /////////////////////////////////////////////////////////////////////

	// ���_�V�F�[�_(VertexShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//�t�@�C�����J���Ȃ������ꍇ�A�������I������
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::Dimension2].pVertexShader);

	HR_FAILED_RELEASE(hr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���",pCompileVS);

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },					//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV���W
	};

	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::Dimension2].pVertexLayout);
	HR_FAILED(hr, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���");

	SAFE_RELEASE(pCompileVS);		//�R���p�C���������_�V�F�[�_�����

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Simple2D.hlsl", SHADER_TYPE::Dimension2);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::Dimension2].pRasterizerState);

	HR_FAILED(hr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���");

	return hr;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	///////////////////////////////////////////////// 3D�p /////////////////////////////////////////////////////////////////

	// ���_�V�F�[�_(VertexShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//�t�@�C�����J���Ȃ������ꍇ�A�������I������
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::Dimension3].pVertexShader);
	HR_FAILED_RELEASE(hr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", pCompileVS);

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV���W
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�@��
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::Dimension3].pVertexLayout);

	HR_FAILED(hr, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���");

	SAFE_RELEASE(pCompileVS);		//�R���p�C���������_�V�F�[�_�����

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Simple3D.hlsl", SHADER_TYPE::Dimension3);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::Dimension3].pRasterizerState);

	HR_FAILED(hr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���");
	return hr;
}

HRESULT Direct3D::InitShaderPoint()
{
	HRESULT hr;

	// ���_�V�F�[�_(VertexShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Point3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//�t�@�C�����J���Ȃ������ꍇ�A�������I������
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::POINT].pVertexShader);
	HR_FAILED_RELEASE(hr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", pCompileVS);

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV���W
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�@��
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::POINT].pVertexLayout);

	HR_FAILED(hr, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���");

	SAFE_RELEASE(pCompileVS);		//�R���p�C���������_�V�F�[�_�����

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Point3D.hlsl", SHADER_TYPE::POINT);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::POINT].pRasterizerState);

	HR_FAILED(hr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���");
	return hr;
}

HRESULT Direct3D::InitShaderFog()
{
	HRESULT hr;

	// ���_�V�F�[�_(VertexShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Fog.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//�t�@�C�����J���Ȃ������ꍇ�A�������I������
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::FOG].pVertexShader);
	HR_FAILED_RELEASE(hr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", pCompileVS);

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV���W
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�@��
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::FOG].pVertexLayout);

	HR_FAILED(hr, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���");

	SAFE_RELEASE(pCompileVS);		//�R���p�C���������_�V�F�[�_�����

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Fog.hlsl", SHADER_TYPE::FOG);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::FOG].pRasterizerState);

	HR_FAILED(hr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���");
	return hr;
}

HRESULT Direct3D::InitShaderBillboard()
{
	HRESULT hr;

	// ���_�V�F�[�_(VertexShader)�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Billboard.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//�t�@�C�����J���Ȃ������ꍇ�A�������I������
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::BILLBOARD].pVertexShader);
	HR_FAILED_RELEASE(hr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", pCompileVS);

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV���W
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::BILLBOARD].pVertexLayout);

	HR_FAILED(hr, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���");

	SAFE_RELEASE(pCompileVS);		//�R���p�C���������_�V�F�[�_�����

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Billboard.hlsl", SHADER_TYPE::BILLBOARD);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::BILLBOARD].pRasterizerState);

	HR_FAILED(hr, L"���X�^���C�U�̍쐬�Ɏ��s���܂���");
	return hr;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	pContext->VSSetShader(shader[(int)type].pVertexShader, NULL, 0);	//���_�V�F�[�_�[
	pContext->PSSetShader(shader[(int)type].pPixelShader, NULL, 0);		//�s�N�Z���V�F�[�_�[
	pContext->IASetInputLayout(shader[(int)type].pVertexLayout);		//���_�C���v�b�g���C�A�E�g
	pContext->RSSetState(shader[(int)type].pRasterizerState);			//���X�^���C�U
}

void Direct3D::SetBlendMode(char blendMode)
{
	//���Z����
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pContext->OMSetBlendState(pBlendState[blendMode], blendFactor, UINT_MAX);

	//Z�o�b�t�@�ւ̏�������
	pContext->OMSetDepthStencilState(pDepthStencilState[blendMode], 0);
}

//�`��J�n
void Direct3D::BeginDraw()
{
	//��ʂ��N���A
	pContext->ClearRenderTargetView(pRenderTargetView, BackGroundColor);
	
	Camera::Update();

	//�[�x�o�b�t�@�N���A
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//�`��I��
void Direct3D::EndDraw()
{
	//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	pSwapChain->Present(0, 0);
}

//�������
void Direct3D::Release()
{
	for (auto&& itr : shader) {
		SAFE_RELEASE(itr.pRasterizerState);
		SAFE_RELEASE(itr.pVertexLayout);
		SAFE_RELEASE(itr.pPixelShader);
		SAFE_RELEASE(itr.pVertexShader);
	}
	for (auto&& itr : pBlendState) {
		SAFE_RELEASE(itr);
	}
	for (auto&& itr : pDepthStencilState) {
		SAFE_RELEASE(itr);
	}
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);
}

void Direct3D::SetViewPort(char lr)
{
	pContext->RSSetViewports(1, &vp[lr]);
}