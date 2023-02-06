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
	int SelectingImage = -1;	//�I�𒆂̉摜�ԍ�
	XMFLOAT3 pos = {};			//�I�����Ă���摜�̈ʒu
	XMFLOAT3 InitialPos = {};	//�I�������摜�̏����ʒu
	int MovementUnit = 1;		//�ړ��P��
}

namespace Debug
{
	void Overwrite();	//�摜�ʒu�̏㏑��
}

namespace Debug
{
	//�摜�̈ʒu����
	void ImagePositioning()
	{
		if (GameTime::IsStop())
		{
			ImGui::Begin("ImageData");
			ImGui::Text("Selecting = %d", SelectingImage);
			ImGui::SliderInt("MovementUnit", &MovementUnit, 1, 100);

			if (Input::IsMouse(0))
			{
				//�}�E�X�����N���b�N�������_�őI�����Ă���摜�ԍ�������
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

				//���炩�̉摜��I�����Ă���ꍇ
				if (SelectingImage != -1)
				{
					pos = Input::GetMousePosition();
					pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

					//�ړ��P�ʂŒ���
					pos = { pos.x - ((int)pos.x % MovementUnit), pos.y - ((int)pos.y % MovementUnit) , pos.z };

					//Transform�P�ʂɕϊ�
					XMFLOAT3 trans = Math::PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			if (ImGui::Button("Save"))
				Overwrite();

			//��������̉摜��I�����Ă���΂��̉摜�̈ʒu���\�������
			if (SelectingImage != -1)
				ImGui::Text("position x = %g, y = %g", pos.x, pos.y);
			ImGui::End();
		}
	}

	void Overwrite()
	{
		//��Ŗ��O��ݒ�ł���悤�ɂ���
		WritePrivateProfileStringA("IMAGE", "Easy_x", std::to_string((int)pos.x).c_str(), "Assets\\Setting.ini");
		WritePrivateProfileStringA("IMAGE", "Easy_y", std::to_string((int)pos.y).c_str(), "Assets\\Setting.ini");
		//WritePrivateProfileStringA("IMAGE", "Hard_x", 0, "Assets\\Setting.ini");
		//WritePrivateProfileStringA("IMAGE", "Hard_y", 0, "Assets\\Setting.ini");
	}
};

#endif