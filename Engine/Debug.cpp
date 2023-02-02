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
	int MovementUnit = 1;		//�ړ��P��
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
				}

				//���炩�̉摜��I�����Ă���ꍇ
				if (SelectingImage != -1)
				{
					//�}�E�X�̈ړ��������擾
					pos = Input::GetMousePosition();

					//�ړ��P�ʂŒ���
					pos = { pos.x - ((int)pos.x % MovementUnit), pos.y - ((int)pos.y % MovementUnit) , pos.z };

					//Transform�P�ʂɕϊ�
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