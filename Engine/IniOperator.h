#pragma once
#include <string>

//Ini�t�@�C���̓ǂݍ���/�����o�����Ǘ�����
namespace IniOperator
{
	/// <summary>�t�@�C�����ƃZ�N�V�������̃Z�b�g�����X�g�ɒǉ�����</summary>
	/// <param name="filename">�ǂݍ��ރt�@�C����</param>
	/// <param name="sectionname">�Z�N�V������</param>
	/// <returns>�쐬�����z��̗v�f�ԍ�</returns>
	int AddList(const std::string& filename, const std::string& sectionname);

	/// <summary>ini�t�@�C������w�肵���L�[�̒l���擾</summary>
	/// <param name="Listnum">AddList�Ŏ擾�����l</param>
	/// <param name="Key">�����������L�[��</param>
	/// <param name="SpareValue">������Ȃ������ꍇ�̒l</param>
	/// <returns>�擾�����l</returns>
	int GetValue(int Listnum, const std::string& Key, int SpareValue = -1);

	/// <summary>ini�t�@�C���̎w�肵���L�[�̒l���㏑��</summary>
	/// <param name="Listnum">AddList�Ŏ擾�����l</param>
	/// <param name="Key">�ύX�������L�[��</param>
	/// <param name="Value">�L�[�ɐݒ肷��l</param>
	void SetValue(int Listnum, const std::string& Key, int Value);
};