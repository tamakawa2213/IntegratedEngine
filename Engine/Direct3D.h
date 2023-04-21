#pragma once
//#ifndef _DIRECT3D_H_
//#define _DIRECT3D_H_

#include <assert.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum class SHADER_TYPE {
	Dimension2 = 0,	//2D用シェーダー
	Dimension3,		//3D用シェーダー
	POINT,			//点光源シェーダー
	FOG,			//フォグ(霧)
	BILLBOARD,
	MAX				//これは使わない
};

enum VP_TYPE
{
	VP_FULL,	//全画面描画
	VP_LEFT,	//画面左描画
	VP_RIGHT,	//画面右描画
	VP_MAX
};

enum class SCREEN_MODE
{
	FULL,		//画面分割なし
	SPLIT_2,		//二画面
	MAX
};

enum BLEND_MODE	//ブレンドモードの指定
{
	BLEND_DEFAULT,
	BLEND_ADD,
	BLEND_MAX
};

namespace Direct3D
{
	//extern = どこかで生成されていて、複数回生成されないようにするためのもの
	extern ID3D11Device* pDevice;			//デバイス
	extern ID3D11DeviceContext* pContext;	//デバイスコンテキスト
	extern IDXGISwapChain* pSwapChain;		//スワップチェイン
	extern int scrWidth, scrHeight;			//スクリーンの幅と高さ
	extern SCREEN_MODE SplitScrMode;
	extern float BackGroundColor[4];

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	

	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	void SetViewPort(char lr);

	void SetBlendMode(char blendMode);
};

//#endif

