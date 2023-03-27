#include "Debug.h"

#include "Direct3D.h"
#include "Image.h"
#include "IniOperator.h"
#include "Input.h"
#include "GameTime.h"
#include "Math.h"
#include "Text.h"
#include "../Graphics/imgui.h"
#include <map>

//外部から呼び出させない関数の前方宣言
namespace Debug
{
	//画像の位置調整
	void ImagePositioning();

	//画像位置の上書き
	void Overwrite();

	//指定したピクセルを始点とする文字情報を出力
	void WriteText();
}

namespace
{
	int SelectingImage = -1;	//選択中の画像番号
	XMFLOAT3 pos = {};			//選択している画像の位置
	XMFLOAT3 InitialPos = {};	//選択した画像の初期位置
	int MovementUnit_x = 1;		//x軸の移動単位
	int MovementUnit_y = 1;		//y軸の移動単位
	std::map<int, XMFLOAT3> ChangedImageStatus = {};	//変更を加えた画像番号

	//デバッグモードをキーとして関数を呼び出すmap
	std::map<DEBUG_MODE, void(*)()> data = {
		{DEBUG_MODE::IMAGE_POSITIONING, &Debug::ImagePositioning},
		{DEBUG_MODE::TEXT_WRITING, &Debug::WriteText}
	};

	DEBUG_MODE Mode = DEBUG_MODE::IMAGE_POSITIONING;	//現在のデバッグモード

	FontData* font;	//文字情報を格納する構造体
	Text text(font);
}


namespace Debug
{
	bool CallDebug_ = false;	//trueならデバッグモードを表示

	//画像の位置調整
	void ImagePositioning()
	{
		ImGui::Begin("ImageData");
		ImGui::Text("Selecting = %d", SelectingImage);
		ImGui::SliderInt("MovementUnit_x", &MovementUnit_x, 1, 100);
		ImGui::SliderInt("MovementUnit_y", &MovementUnit_y, 1, 100);

		if (ImGui::Button("Save"))
			Overwrite();

		if (Input::Mouse::IsPush(0))
		{
			//マウスを左クリックした時点で選択している画像番号が入る
			if (Input::Mouse::Down(0))
			{
				SelectingImage = Image::IsHitCursorAny();

				if (SelectingImage != -1)
				{
					XMFLOAT3 iPos = Math::TransformToPixel(Image::GetPosition(SelectingImage));
					XMFLOAT3 mPos = Input::Mouse::GetPosition();
					InitialPos = { mPos.x - iPos.x, mPos.y - iPos.y,0 };
				}
			}

			//何らかの画像を選択している場合
			if (SelectingImage != -1)
			{
				pos = Input::Mouse::GetPosition();
				pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

				//移動単位で調整
				pos = { (float)((int)pos.x - ((int)pos.x % MovementUnit_x)), (float)((int)pos.y - ((int)pos.y % MovementUnit_y)) , pos.z };

				//Transform単位に変換
				XMFLOAT3 trans = Math::PixelToTransform(pos);

				Image::SetPosition(SelectingImage, trans);
			}
		}

		//マウスを離した時にその画像の位置が一時保存される
		if (Input::Mouse::Up(0))
		{
			if (SelectingImage != -1)
			{
				ChangedImageStatus.insert_or_assign(SelectingImage, pos);
			}
		}

		//何かしらの画像を選択していればその画像の位置が表示される
		if (SelectingImage != -1)
			ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
		ImGui::End();

	}

	void BranchMode()
	{
		if (CallDebug_)
		{
			/*ImGui::Begin("DebugMode");
			ImGui::BeginCombo("Mode", "ImagePositioning");
			ImGui::Combo("Mode", (int*)Mode, "WriteText");
			ImGui::EndCombo();*/

			data[Mode]();
		}
	}

	void SetDebugMode(DEBUG_MODE mode)
	{
		Mode = mode;
	}

	void Overwrite()
	{
		for (auto itr : ChangedImageStatus)
		{
			//"Assets\\"を省いた文字列を取得
			std::string file = Image::GetFileName(itr.first).substr(7);
			int num = IniOperator::AddList("Assets\\ImageStatus.ini", file);
			IniOperator::SetValue(num, "x", (int)itr.second.x);
			IniOperator::SetValue(num, "y", (int)itr.second.y);
		}
		ChangedImageStatus.clear();
	}

	void WriteText()
	{
		//ImGui::Begin("TextData");
		//ImGui::InputFloat("TextSize", (float*)&font->fontSize);	//フォントサイズを変更
		//char* data;	//文字情報を格納する
		//ImGui::InputText("TextBox", data, sizeof(data));
		//ImGui::End();
	}
};