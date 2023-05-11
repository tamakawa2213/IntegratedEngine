#include "Debug.h"

#include "Direct3D.h"
#include "Image.h"
#include "IniOperator.h"
#include "Input.h"
#include "GameTime.h"
#include "Math.h"
#include "Text.h"
#include <imgui.h>
#include <filesystem>
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
	XMINT3 pos = {};			//�I�����Ă���摜�̈ʒu
	XMINT3 InitialPos = {};	//�I�������摜�̏����ʒu
	int MovementUnit_x = 1;		//x���̈ړ��P��
	int MovementUnit_y = 1;		//y���̈ړ��P��
	std::map<int, XMINT3> ChangedImageStatus = {};	//�ύX���������摜�ԍ�

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
					XMINT3 mPos = Input::Mouse::GetPosition();
					InitialPos = { mPos.x - (int)iPos.x, mPos.y - (int)iPos.y,0 };
				}
			}

			//���炩�̉摜��I�����Ă���ꍇ
			if (SelectingImage != -1)
			{
				pos = Input::Mouse::GetPosition();
				pos = { pos.x - InitialPos.x, pos.y - InitialPos.y, 0 };

				//�ړ��P�ʂŒ���
				pos = { pos.x - (pos.x % MovementUnit_x), pos.y - (pos.y % MovementUnit_y), pos.z };

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
			ImGui::Text("position x = %d, y = %d", pos.x, pos.y);
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
		for (auto&& itr : ChangedImageStatus)
		{
			//"Assets\\"���Ȃ�����������擾
			std::filesystem::path file = Image::GetFileName(itr.first);
			int num = IniOperator::AddList("Assets\\ImageStatus.ini", file.filename().string());
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
}; // namespace Debug