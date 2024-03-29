#include "Input.h"
#include "CallDef.h"
#include <assert.h>
#include <algorithm>
#include <thread>

namespace Input
{
	//キーボード
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;	//デバイスオブジェクト
	BYTE keyState[256] = { 0 };					//現在の各キーの状態
	BYTE prevKeyState[256];						//前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;	//マウス
	DIMOUSESTATE mouseState;						//マウスの状態
	DIMOUSESTATE prevMouseState;					//前フレームのマウスの状態
	DirectX::XMINT3 mousePosition;				//マウスの位置
	DirectX::XMINT3 prevMousePosition;
	int MouseWheel = 0;

	//コントローラー
	XINPUT_STATE controllerState_;
	XINPUT_STATE prevControllerState_;
}

namespace Input
{
	void Initialize(HWND hWnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		assert(hr == S_OK);

		//キーボード
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		assert(hr == S_OK);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//マウス
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		assert(hr == S_OK);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		pMouseDevice->Acquire();
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		//コントローラー
		prevControllerState_ = controllerState_;
		XInputGetState(0, &controllerState_);
	}

	///////////////////////////////////////////////////キーボード関連///////////////////////////////////////////
	namespace Keyboard
	{
		bool IsPush(int keyCode)
		{
			if (keyState[keyCode] & 0x80)	//16進数80との論理積
			{
				return true;
			}

			return false;
		}

		bool Down(int keyCode)
		{
			//今は押してて、前回は押してない
			if (IsPush(keyCode) && !(prevKeyState[keyCode] & 0x80))
			{
				return true;
			}
			return false;
		}

		bool Up(int keyCode)
		{
			//前回は押してて、今は押してない
			if ((prevKeyState[keyCode] & 0x80) && !IsPush(keyCode))
			{
				return true;
			}
			return false;
		}


		bool AnyPush()
		{
			if (std::find(keyState, keyState + 256, 0x80) != keyState + 256)
			{
				return true;
			}
			return false;
		}

		bool AnyDown()
		{
			if (AnyPush() && !(std::find(prevKeyState, prevKeyState + 256, 0x80) != prevKeyState + 256))
			{
				return true;
			}
			return false;
		}

		bool AnyUp()
		{
			if ((std::find(prevKeyState, prevKeyState + 256, 0x80) != prevKeyState + 256) && !AnyPush())
			{
				return true;
			}
			return false;
		}

		bool Add()
		{
			int push, prev;
			std::thread th_a([&] { push = (int)std::count(keyState, keyState + 256, 0x80); });
			std::thread th_b([&] { prev = (int)std::count(prevKeyState, prevKeyState + 256, 0x80); });
			th_a.join();
			th_b.join();
			if (push > prev)
			{
				return true;
			}
			return false;
		}

		bool Remove()
		{
			int push, prev;
			std::thread th_a([&] { push = (int)std::count(keyState, keyState + 256, 0x80); });
			std::thread th_b([&] { prev = (int)std::count(prevKeyState, prevKeyState + 256, 0x80); });
			th_a.join();
			th_b.join();
			if (push < prev)
			{
				return true;
			}
			return false;
		}
	}

	///////////////////////////////////////////////////////マウス関連//////////////////////////////////////////////////
	namespace Mouse
	{
		DirectX::XMINT3 GetPosition()
		{
			return mousePosition;
		}

		void SetPosition(int x, int y)
		{
			prevMousePosition = mousePosition;
			mousePosition = { x, y, 0 };
		}

		void SetWheel(int r)
		{
			MouseWheel = r;
		}

		int GetWheel()
		{
			int r = MouseWheel;
			MouseWheel = 0;
			return r;
		}

		DirectX::XMINT3 GetMovement()
		{
			DirectX::XMINT3 MouseMovement{};
			MouseMovement.x = mousePosition.x - prevMousePosition.x;
			MouseMovement.y = mousePosition.y - prevMousePosition.y;
			return MouseMovement;
		}

		bool IsMove()
		{
			DirectX::XMINT2 MouseMovement{};
			MouseMovement.x = mousePosition.x - prevMousePosition.x;
			MouseMovement.y = mousePosition.y - prevMousePosition.y;
			if (MouseMovement.x == 0 && MouseMovement.y == 0) {
				return false;
			}
			return true;
		}

		bool IsPush(int mouseBotton)
		{
			if (mouseState.rgbButtons[mouseBotton] & 0x80)	//16進数80との論理積
			{
				return true;
			}
			return false;
		}

		bool Down(int mouseBotton)
		{
			if (IsPush(mouseBotton) && !(prevMouseState.rgbButtons[mouseBotton] & 0x80))
			{
				return true;
			}
			return false;
		}

		bool Up(int mouseBotton)
		{
			if (!IsPush(mouseBotton) && prevMouseState.rgbButtons[mouseBotton] & 0x80)
			{
				return true;
			}
			return false;
		}
	}

	//////////////////////////////////////////コントローラー////////////////////////////////////////////
	namespace Controller
	{
		bool IsPush(int ctrlBotton)
		{
			if (controllerState_.Gamepad.wButtons & ctrlBotton)
			{
				return true;
			}
			return false;
		}

		bool Up(int ctrlBotton)
		{
			if (IsPush(ctrlBotton) && !(prevControllerState_.Gamepad.wButtons & ctrlBotton))
			{
				return true;
			}
			return false;
		}

		bool Down(int ctrlBotton)
		{
			if (!IsPush(ctrlBotton) && prevControllerState_.Gamepad.wButtons & ctrlBotton)
			{
				return true;
			}
			return false;
		}

		DirectX::XMFLOAT3 Inclination_StickL()
		{
			if ((controllerState_.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				controllerState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(controllerState_.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					controllerState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				controllerState_.Gamepad.sThumbLX = 0;
				controllerState_.Gamepad.sThumbLY = 0;
			}
			float x = (float)controllerState_.Gamepad.sThumbLX / 32767.0f;
			float y = (float)controllerState_.Gamepad.sThumbLY / 32767.0f;
			return DirectX::XMFLOAT3(x, y, 0);
		}

		DirectX::XMFLOAT3 Inclination_StickR()
		{
			if ((controllerState_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				controllerState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(controllerState_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					controllerState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				controllerState_.Gamepad.sThumbRX = 0;
				controllerState_.Gamepad.sThumbRY = 0;
			}
			float x = (float)controllerState_.Gamepad.sThumbRX / 32767.0f;
			float y = (float)controllerState_.Gamepad.sThumbRY / 32767.0f;
			return DirectX::XMFLOAT3(x, y, 0);
		}

		float Inclination_Trigger(BYTE Trigger)
		{
			float value = Trigger;
			if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				value = 0.0f;
			}
			value = value / 255;
			return value;
		}
	}

	void Release()
	{
		SAFE_RELEASE(pMouseDevice);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}
}