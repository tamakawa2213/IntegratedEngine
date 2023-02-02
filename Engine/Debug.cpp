#include "Debug.h"

#if _DEBUG
#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "GameTime.h"
#include "../Graphics/imgui.h"


namespace
{
	int SelectingImage = -1;	//選択中の画像番号
	XMFLOAT3 pos = {};			//選択している画像の位置
}

namespace Debug
{
	//Transformの位置をピクセルに変換
	XMFLOAT3 TransformToPixel(XMFLOAT3 transform);
	//ピクセルをTransformに変換
	XMFLOAT3 PixelToTransform(XMFLOAT3 pixel);
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
					//選択画像の位置を取得
					pos = Image::GetPosition(SelectingImage);

					//取得した画像をピクセル単位に変換
					pos = TransformToPixel(pos);

					//マウスの移動距離を取得
					//pos = { pos.x + Input::GetMouseMovement().x, pos.y + Input::GetMouseMovement().y, pos.z };
					pos = Input::GetMousePosition();

					//Transform単位に変換
					XMFLOAT3 trans = PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			if (SelectingImage != -1)
				ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
			ImGui::End();
		}
	}


	XMFLOAT3 TransformToPixel(XMFLOAT3 transform)
	{
		//position_を0~1に変更
		XMFLOAT3 pos = { (transform.x + 1) / 2, (-transform.y + 1) / 2, transform.z };

		pos = { pos.x * Direct3D::scrWidth, pos.y * Direct3D::scrHeight, pos.z };

		return pos;
	}

	XMFLOAT3 PixelToTransform(XMFLOAT3 pixel)
	{
		XMFLOAT3 pos = { pixel.x / Direct3D::scrWidth, pixel.y / Direct3D::scrHeight, pixel.z };
		pos = { pos.x * 2 - 1, -pos.y * 2 + 1, pos.z };
		return pos;
	}
};

#endif