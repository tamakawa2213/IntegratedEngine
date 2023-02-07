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
	int SelectingImage = -1;	//�I�𒆂̉摜�ԍ�
	XMFLOAT3 pos = {};			//�I�����Ă���摜�̈ʒu
	XMFLOAT3 InitialPos = {};	//�I�������摜�̏����ʒu
	int MovementUnit_x = 1;		//x���̈ړ��P��
	int MovementUnit_y = 1;		//y���̈ړ��P��
	std::list<int> ChangedImageStatus = {};	//�ύX���������摜�ԍ�
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
			ImGui::SliderInt("MovementUnit_x", &MovementUnit_x, 1, 100);
			ImGui::SliderInt("MovementUnit_y", &MovementUnit_y, 1, 100);

			ImGui::Text("You can save by pressing ctrl + S");
			if ((Input::IsKey(DIK_LCONTROL) || Input::IsKey(DIK_RCONTROL)) && Input::IsKeyDown(DIK_S))
				Overwrite();

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
						ChangedImageStatus.push_back(SelectingImage);
					}
				}

				//���炩�̉摜��I�����Ă���ꍇ
				if (SelectingImage != -1)
				{
					pos = Input::GetMousePosition();
					pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

					//�ړ��P�ʂŒ���
					pos = { pos.x - ((int)pos.x % MovementUnit_x), pos.y - ((int)pos.y % MovementUnit_y) , pos.z };

					//Transform�P�ʂɕϊ�
					XMFLOAT3 trans = Math::PixelToTransform(pos);

					Image::SetPosition(SelectingImage, trans);
				}
			}

			//��������̉摜��I�����Ă���΂��̉摜�̈ʒu���\�������
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