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
	int MovementUnit = 1;		//移動単位
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
				}

				//何らかの画像を選択している場合
				if (SelectingImage != -1)
				{
					//マウスの移動距離を取得
					pos = Input::GetMousePosition();

					//移動単位で調整
					pos = { pos.x - ((int)pos.x % MovementUnit), pos.y - ((int)pos.y % MovementUnit) , pos.z };

					//Transform単位に変換
					XMFLOAT3 trans = Math::PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			if (SelectingImage != -1)
				ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
			ImGui::End();
		}
	}
};

#endif