#include "Text.h"
#include "CallDef.h"
#include "Direct3D.h"
#include <memory>

std::wstring Text::StringToWString(const std::string& str)
{
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

	std::unique_ptr<wchar_t[]> cpUCS2 = std::make_unique<wchar_t[]>(iBufferSize);

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, cpUCS2.get(), iBufferSize);

	std::wstring Ret(std::move(cpUCS2.get()), cpUCS2.get() + iBufferSize - 1);

	return Ret;
}

Text::~Text()
{
}

void Text::SetFont(FontData* set)
{
	pDWriteFactory->CreateTextFormat(FontList[(int)set->font], set->fontCollection, set->fontWeight, set->fontStyle, set->fontStretch, set->fontSize, set->localeName, &pTextFormat);
	pTextFormat->SetTextAlignment(set->textAlignment);
	pRT->CreateSolidColorBrush(set->Color, &pSolidBrush);
}

void Text::Initialize()
{
	//Direct2D, DirectWrite�̏�����
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	Direct3D::pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	//�����_�[�^�[�Q�b�g�̍쐬
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), (FLOAT)Direct3D::scrWidth, (FLOAT)Direct3D::scrHeight);

	//�T�[�t�F�X�ɕ`�悷�郌���_�[�^�[�Q�b�g���쐬
	pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &pRT);

	//�A���`�G�C���A�V���O���[�h
	pRT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	//IDWriteFactory�̍쐬
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

	SetFont(Setting);
}

void Text::Release()
{
	SAFE_RELEASE(pTextLayout);
	SAFE_RELEASE(pSolidBrush);
	SAFE_RELEASE(pTextFormat);
	SAFE_RELEASE(pDWriteFactory);
	SAFE_RELEASE(pRT);
	SAFE_RELEASE(pBackBuffer);
	SAFE_RELEASE(pD2DFactory);
}