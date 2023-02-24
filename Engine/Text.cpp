#include "Text.h"
#include "CallDef.h"
#include "Direct3D.h"

std::wstring Text::StringToWString(std::string str)
{
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, cpUCS2, iBufferSize);

	std::wstring Ret(cpUCS2, cpUCS2 + iBufferSize - 1);

	SAFE_DELETE_ARRAY(cpUCS2);

	return Ret;
}

Text::~Text()
{
	Release();
}

void Text::SetFont(FontData* set)
{
	pDWriteFactory->CreateTextFormat(FontList[(int)set->font], set->fontCollection, set->fontWeight, set->fontStyle, set->fontStretch, set->fontSize, set->localeName, &pTextFormat);
	pTextFormat->SetTextAlignment(set->textAlignment);
	pRT->CreateSolidColorBrush(set->Color, &pSolidBrush);
}

void Text::Initialize()
{
	//Direct2D, DirectWriteの初期化
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	Direct3D::pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	//レンダーターゲットの作成
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), (FLOAT)Direct3D::scrWidth, (FLOAT)Direct3D::scrHeight);

	//サーフェスに描画するレンダーターゲットを作成
	pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer, &props, &pRT);

	//アンチエイリアシングモード
	pRT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	//IDWriteFactoryの作成
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

	pDWriteFactory->CreateTextFormat(FontList[(int)Setting->font],
		Setting->fontCollection,
		Setting->fontWeight,
		Setting->fontStyle,
		Setting->fontStretch,
		Setting->fontSize,
		Setting->localeName,
		&pTextFormat);

	pTextFormat->SetTextAlignment(Setting->textAlignment);

	pRT->CreateSolidColorBrush(Setting->Color, &pSolidBrush);
}

void Text::Release()
{
	SAFE_RELEASE(pBackBuffer);
	SAFE_RELEASE(pSolidBrush);
	SAFE_RELEASE(pRT);
	SAFE_RELEASE(pTextFormat);
	SAFE_RELEASE(pDWriteFactory);
	SAFE_RELEASE(pD2DFactory);
	SAFE_RELEASE(pTextLayout);
}