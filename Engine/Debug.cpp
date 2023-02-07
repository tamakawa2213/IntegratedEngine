#include "Debug.h"

#if _DEBUG
#include "Direct3D.h"
#include "Image.h"
#include "IniOperator.h"
#include "Input.h"
#include "GameTime.h"
#include "Math.h"
#include "../Graphics/imgui.h"
#include <list>


namespace
{
	int SelectingImage = -1;	//選択中の画像番号
	XMFLOAT3 pos = {};			//選択している画像の位置
	XMFLOAT3 InitialPos = {};	//選択した画像の初期位置
	int MovementUnit_x = 1;		//x軸の移動単位
	int MovementUnit_y = 1;		//y軸の移動単位
	std::list<int> ChangedImageStatus = {};	//変更を加えた画像番号
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
			ImGui::SliderInt("MovementUnit_x", &MovementUnit_x, 1, 100);
			ImGui::SliderInt("MovementUnit_y", &MovementUnit_y, 1, 100);

			ImGui::Text("You can save by pressing ctrl + S");
			if ((Input::IsKey(DIK_LCONTROL) || Input::IsKey(DIK_RCONTROL)) && Input::IsKeyDown(DIK_S))
				Overwrite();

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
						ChangedImageStatus.push_back(SelectingImage);
					}
				}

				//何らかの画像を選択している場合
				if (SelectingImage != -1)
				{
					pos = Input::GetMousePosition();
					pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

					//移動単位で調整
					pos = { pos.x - ((int)pos.x % MovementUnit_x), pos.y - ((int)pos.y % MovementUnit_y) , pos.z };

					//Transform単位に変換
					XMFLOAT3 trans = Math::PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			//何かしらの画像を選択していればその画像の位置が表示される
			if (SelectingImage != -1)
				ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
			ImGui::End();
		}
	}

	void Overwrite()
	{
		ChangedImageStatus.unique();
		for (auto itr : ChangedImageStatus)
		{
			//LPCSTR file = (LPCSTR)Image::GetFileName(itr);
			IniOperator::SetValue(0, "x", (int)pos.x);
			IniOperator::SetValue(0, "y", (int)pos.y);
		}
		ChangedImageStatus.clear();
	}
};

#endif