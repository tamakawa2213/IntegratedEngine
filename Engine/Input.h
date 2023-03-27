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

	//キーボード
	namespace Keyboard
	{
		bool IsPush(int keyCode);		//キーを押している間
		bool Down(int keyCode);			//キーを押した瞬間
		bool Up(int keyCode);			//キーを離した瞬間
		bool AnyPush();					//いずれかのキーを押している間
		bool AnyDown();					//いずれかのキーを押した瞬間
		bool AnyUp();					//いずれかのキーを離した瞬間
		bool Add();						//押しているキーが前回よりも多いか
		bool Remove();					//押しているキーが前回よりも少ないか
	}

	//マウス
	namespace Mouse
	{
		DirectX::XMFLOAT3 GetPosition();		//マウスの位置を取得
		void SetPosition(int x, int y);			//マウスの位置を設定
		DirectX::XMFLOAT3 GetMovement();		//マウスの移動量を取得
		void SetWheel(int r);					//マウスホイールの回転量を設定
		int GetWheel();							//マウスホイールの回転量を取得
		bool IsMove();							//マウスが動いたか
		bool IsPush(int mouseBotton);			//マウスを押している間
		bool Down(int mouseBotton);				//マウスを押した瞬間
		bool Up(int mouseBotton);				//マウスを離した瞬間
	}

	//コントローラー
	namespace Controller
	{
		bool IsPush(int ctrlBotton);				//ボタンを押している間
		bool Up(int ctrlBotton);					//ボタンを離した瞬間
		bool Down(int ctrlBotton);					//ボタンを押した瞬間
		DirectX::XMFLOAT3 Inclination_StickL();		//Lスティックの傾き
		DirectX::XMFLOAT3 Inclination_StickR();		//Rスティックの傾き
		float Inclination_Trigger(BYTE Trigger);	//トリガーの押し込み具合
	}

	void Release();
};