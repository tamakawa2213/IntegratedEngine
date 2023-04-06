#include "Direct3D.h"
#include "Camera.h"
#include "CallDef.h"
#include <d3dcompiler.h>
#include <iterator>

//変数
namespace Direct3D
{
	//Direct3D関連
	ID3D11Device* pDevice = nullptr;						//デバイス
	ID3D11DeviceContext* pContext = nullptr;				//デバイスコンテキスト
	IDXGISwapChain* pSwapChain = nullptr;					//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView = nullptr;	//レンダーターゲットビュー
	ID3D11BlendState* pBlendState[BLEND_MAX];				//ブレンドステート
	ID3D11Texture2D* pDepthStencil = nullptr;				//深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView = nullptr;	//深度ステンシルビュー
	ID3D11DepthStencilState* pDepthStencilState[BLEND_MAX];

	//シェーダ関連
	struct SHADER {
		ID3D11VertexShader* pVertexShader = nullptr;		//頂点シェーダー
		ID3D11PixelShader* pPixelShader = nullptr;			//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout = nullptr;			//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー
	};
	SHADER shader[(int)SHADER_TYPE::MAX];

	int scrWidth, scrHeight;

	char SplitScrMode = SCREEN_FULL;

	//背景色
	float BackGroundColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	D3D11_VIEWPORT vp[VP_MAX];
}

namespace Direct3D
{
	//シェーダーの初期化
	HRESULT InitShader2D();
	HRESULT InitShader3D();
	HRESULT InitShaderPoint();
	HRESULT InitShaderFog();
	HRESULT InitShaderBillboard();
	HRESULT InitPixelShader(LPCWSTR filename, SHADER_TYPE type);	//ピクセルシェーダの作成
}

//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	scrWidth = winW;
	scrHeight = winH;

	HRESULT hr;

	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,						// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,								// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,						// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,								// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,				// SDKのバージョン。必ずこの値
		&scDesc,						// 上でいろいろ設定した構造体
		&pSwapChain,					// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,						// 無事完成したDeviceアドレスが返ってくる
		&level,							// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext);						// 無事完成したContextのアドレスが返ってくる
	HR_FAILED(hr, L"デバイス、コンテキスト、スワップチェインの作成に失敗しました");

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	assert(pBackBuffer != nullptr);

	//レンダーターゲットビューを作成
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	HR_FAILED_RELEASE(hr, L"レンダーターゲットビューの作成に失敗しました", pBackBuffer);
	
	//レンダリング結果を表示する範囲
	vp[VP_FULL].Width = (float)winW;	//幅
	vp[VP_FULL].Height = (float)winH;	//高さ
	vp[VP_FULL].MinDepth = 0.0f;		//手前
	vp[VP_FULL].MaxDepth = 1.0f;		//奥
	vp[VP_FULL].TopLeftX = 0;			//左
	vp[VP_FULL].TopLeftY = 0;			//上

	vp[VP_LEFT].Width = (float)winW / 2;//幅
	vp[VP_LEFT].Height = (float)winH;	//高さ
	vp[VP_LEFT].MinDepth = 0.0f;		//手前
	vp[VP_LEFT].MaxDepth = 1.0f;		//奥
	vp[VP_LEFT].TopLeftX = 0;			//左
	vp[VP_LEFT].TopLeftY = 0;			//上

	vp[VP_RIGHT].Width = (float)winW / 2;	//幅
	vp[VP_RIGHT].Height = (float)winH;		//高さ
	vp[VP_RIGHT].MinDepth = 0.0f;			//手前
	vp[VP_RIGHT].MaxDepth = 1.0f;			//奥
	vp[VP_RIGHT].TopLeftX = (float)winW / 2;//左
	vp[VP_RIGHT].TopLeftY = 0;				//上

	//深度ステンシルビューの作成
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

	//深度テストを行う深度ステンシルステートの作成
	{
		//デフォルト
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = true;
		pDevice->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_DEFAULT]);
		pContext->OMSetDepthStencilState(pDepthStencilState[BLEND_DEFAULT], 0);

		//加算合成用（書き込みなし）
		desc.StencilEnable = false;
		pDevice->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_ADD]);
	}

	//ブレンドステートの作成
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

		//加算合成（重なるほど光って見える効果）
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pDevice->CreateBlendState(&BlendDesc, &pBlendState[BLEND_ADD]);
	}

	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);		// 描画先を設定
	
	

	//シェーダー準備(処理が長くなったので分割)
	hr = InitShader();
	if (FAILED(hr)) {//失敗したら、hr(原因)をreturnする
		return hr;
	}
	return S_OK;	//HRESULT型の戻り値を返す必要がある
}

//シェーダー準備
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

	// ピクセルシェーダ(PixelShader)の作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(filename, nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shader[(int)type].pPixelShader);

	HR_FAILED_RELEASE(hr, L"ピクセルシェーダの作成に失敗しました", pCompilePS);

	SAFE_RELEASE(pCompilePS);		//コンパイルしたピクセルシェーダを解放

	return hr;
}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	///////////////////////////////////////////////////// 2D用 /////////////////////////////////////////////////////////////////////

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::Dimension2].pVertexShader);

	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました",pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },					//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV座標
	};

	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::Dimension2].pVertexLayout);
	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Simple2D.hlsl", SHADER_TYPE::Dimension2);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::Dimension2].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");

	return hr;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	///////////////////////////////////////////////// 3D用 /////////////////////////////////////////////////////////////////

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::Dimension3].pVertexShader);
	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました", pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV座標
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::Dimension3].pVertexLayout);

	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Simple3D.hlsl", SHADER_TYPE::Dimension3);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::Dimension3].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");
	return hr;
}

HRESULT Direct3D::InitShaderPoint()
{
	HRESULT hr;

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Point3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::POINT].pVertexShader);
	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました", pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV座標
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::POINT].pVertexLayout);

	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Point3D.hlsl", SHADER_TYPE::POINT);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::POINT].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");
	return hr;
}

HRESULT Direct3D::InitShaderFog()
{
	HRESULT hr;

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Fog.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::FOG].pVertexShader);
	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました", pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV座標
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::FOG].pVertexLayout);

	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Fog.hlsl", SHADER_TYPE::FOG);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::FOG].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");
	return hr;
}

HRESULT Direct3D::InitShaderBillboard()
{
	HRESULT hr;

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"../IntegratedEngine/Shader/Billboard.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[(int)SHADER_TYPE::BILLBOARD].pVertexShader);
	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました", pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV座標
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[(int)SHADER_TYPE::BILLBOARD].pVertexLayout);

	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	hr = InitPixelShader(L"../IntegratedEngine/Shader/Billboard.hlsl", SHADER_TYPE::BILLBOARD);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[(int)SHADER_TYPE::BILLBOARD].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");
	return hr;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	//それぞれをデバイスコンテキストにセット
	pContext->VSSetShader(shader[(int)type].pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shader[(int)type].pPixelShader, NULL, 0);		//ピクセルシェーダー
	pContext->IASetInputLayout(shader[(int)type].pVertexLayout);		//頂点インプットレイアウト
	pContext->RSSetState(shader[(int)type].pRasterizerState);			//ラスタライザ
}

void Direct3D::SetBlendMode(char blendMode)
{
	//加算合成
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pContext->OMSetBlendState(pBlendState[blendMode], blendFactor, UINT_MAX);

	//Zバッファへの書き込み
	pContext->OMSetDepthStencilState(pDepthStencilState[blendMode], 0);
}

//描画開始
void Direct3D::BeginDraw()
{
	//画面をクリア
	pContext->ClearRenderTargetView(pRenderTargetView, BackGroundColor);
	
	Camera::Update();

	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//描画終了
void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	pSwapChain->Present(0, 0);
}

//解放処理
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