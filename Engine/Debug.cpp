#include "Debug.h"

#if _DEBUG
#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "GameTime.h"
#include "../Graphics/imgui.h"


namespace
{
	int SelectingImage = -1;	//�I�𒆂̉摜�ԍ�
	XMFLOAT3 pos = {};			//�I�����Ă���摜�̈ʒu
}

namespace Debug
{
	//Transform�̈ʒu���s�N�Z���ɕϊ�
	XMFLOAT3 TransformToPixel(XMFLOAT3 transform);
	//�s�N�Z����Transform�ɕϊ�
	XMFLOAT3 PixelToTransform(XMFLOAT3 pixel);
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
					//�I���摜�̈ʒu���擾
					pos = Image::GetPosition(SelectingImage);

					//�擾�����摜���s�N�Z���P�ʂɕϊ�
					pos = TransformToPixel(pos);

					//�}�E�X�̈ړ��������擾
					//pos = { pos.x + Input::GetMouseMovement().x, pos.y + Input::GetMouseMovement().y, pos.z };
					pos = Input::GetMousePosition();

					//Transform�P�ʂɕϊ�
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
		//position_��0~1�ɕύX
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