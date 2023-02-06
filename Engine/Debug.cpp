#include "Debug.h"

#if _DEBUG
#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "GameTime.h"
#include "Math.h"
#include "../Graphics/imgui.h"


namespace
{
	int SelectingImage = -1;	//選択中の画像番号
	XMFLOAT3 pos = {};			//選択している画像の位置
	XMFLOAT3 InitialPos = {};	//選択した画像の初期位置
	int MovementUnit = 1;		//移動単位
}

namespace Debug
{
	void Overwrite();	//画像位置の上書き
}

namespace Debug
{
	//画像の位置調整
	void ImagePositioning()
	{
		if (GameTime::IsStop())
		{
			ImGui::Begin("ImageData");
			ImGui::Text("Selecting = %d", SelectingImage);
			ImGui::SliderInt("MovementUnit", &MovementUnit, 1, 100);

			if (Input::IsMouse(0))
			{
				//マウスを左クリックした時点で選択している画像番号が入る
				if (Input::IsMouseDown(0))
				{
					SelectingImage = Image::IsHitCursorAny();

					if (SelectingImage != -1)
					{
						XMFLOAT3 iPos = Math::TransformToPixel(Image::GetPosition(SelectingImage));
						XMFLOAT3 mPos = Input::GetMousePosition();
						InitialPos = { mPos.x - iPos.x, mPos.y - iPos.y,0 };
					}
				}

				//何らかの画像を選択している場合
				if (SelectingImage != -1)
				{
					pos = Input::GetMousePosition();
					pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

					//移動単位で調整
					pos = { pos.x - ((int)pos.x % MovementUnit), pos.y - ((int)pos.y % MovementUnit) , pos.z };

					//Transform単位に変換
					XMFLOAT3 trans = Math::PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			if (ImGui::Button("Save"))
				Overwrite();

			//何かしらの画像を選択していればその画像の位置が表示される
			if (SelectingImage != -1)
				ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
			ImGui::End();
		}
	}

	void Overwrite()
	{
		//後で名前を設定できるようにする
		WritePrivateProfileStringA("IMAGE", "Easy_x", std::to_string((int)pos.x).c_str(), "Assets\\Setting.ini");
		WritePrivateProfileStringA("IMAGE", "Easy_y", std::to_string((int)pos.y).c_str(), "Assets\\Setting.ini");
		//WritePrivateProfileStringA("IMAGE", "Hard_x", 0, "Assets\\Setting.ini");
		//WritePrivateProfileStringA("IMAGE", "Hard_y", 0, "Assets\\Setting.ini");
	}
};

#endif