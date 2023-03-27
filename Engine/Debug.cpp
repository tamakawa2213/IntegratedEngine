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

//�O������Ăяo�����Ȃ��֐��̑O���錾
namespace Debug
{
	//�摜�̈ʒu����
	void ImagePositioning();

	//�摜�ʒu�̏㏑��
	void Overwrite();

	//�w�肵���s�N�Z�����n�_�Ƃ��镶�������o��
	void WriteText();
}

namespace
{
	int SelectingImage = -1;	//�I�𒆂̉摜�ԍ�
	XMFLOAT3 pos = {};			//�I�����Ă���摜�̈ʒu
	XMFLOAT3 InitialPos = {};	//�I�������摜�̏����ʒu
	int MovementUnit_x = 1;		//x���̈ړ��P��
	int MovementUnit_y = 1;		//y���̈ړ��P��
	std::map<int, XMFLOAT3> ChangedImageStatus = {};	//�ύX���������摜�ԍ�

	//�f�o�b�O���[�h���L�[�Ƃ��Ċ֐����Ăяo��map
	std::map<DEBUG_MODE, void(*)()> data = {
		{DEBUG_MODE::IMAGE_POSITIONING, &Debug::ImagePositioning},
		{DEBUG_MODE::TEXT_WRITING, &Debug::WriteText}
	};

	DEBUG_MODE Mode = DEBUG_MODE::IMAGE_POSITIONING;	//���݂̃f�o�b�O���[�h

	FontData* font;	//���������i�[����\����
	Text text(font);
}


namespace Debug
{
	bool CallDebug_ = false;	//true�Ȃ�f�o�b�O���[�h��\��

	//�摜�̈ʒu����
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
			//�}�E�X�����N���b�N�������_�őI�����Ă���摜�ԍ�������
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

			//���炩�̉摜��I�����Ă���ꍇ
			if (SelectingImage != -1)
			{
				pos = Input::Mouse::GetPosition();
				pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

				//�ړ��P�ʂŒ���
				pos = { (float)((int)pos.x - ((int)pos.x % MovementUnit_x)), (float)((int)pos.y - ((int)pos.y % MovementUnit_y)) , pos.z };

				//Transform�P�ʂɕϊ�
				XMFLOAT3 trans = Math::PixelToTransform(pos);

				Image::SetPosition(SelectingImage, trans);
			}
		}

		//�}�E�X�𗣂������ɂ��̉摜�̈ʒu���ꎞ�ۑ������
		if (Input::Mouse::Up(0))
		{
			if (SelectingImage != -1)
			{
				ChangedImageStatus.insert_or_assign(SelectingImage, pos);
			}
		}

		//��������̉摜��I�����Ă���΂��̉摜�̈ʒu���\�������
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
			//"Assets\\"���Ȃ�����������擾
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
		//ImGui::InputFloat("TextSize", (float*)&font->fontSize);	//�t�H���g�T�C�Y��ύX
		//char* data;	//���������i�[����
		//ImGui::InputText("TextBox", data, sizeof(data));
		//ImGui::End();
	}
};