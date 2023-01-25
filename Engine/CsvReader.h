#pragma once
#include <string>
#include <vector>

/// <summary>
/// �O��������������Ă���N���X
/// </summary>
class CsvReader {

	//�\����(��s)
	struct LINEREC {
		std::vector<std::string> record;
	};

	//�S�Ă̕����i�[�p
	std::vector<LINEREC> all;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	CsvReader(std::string filename);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CsvReader();

	/// <summary>
	/// �ǂݍ��񂾃t�@�C�������s���邩
	/// </summary>
	/// <returns>���s��</returns>
	int GetLines();

	/// <summary>
	/// �w��s���̒��̑��s����Ԃ�
	/// </summary>
	/// <param name="line">�s��</param>
	/// <returns>�w��s���̒��̑��s��</returns>
	int GetColumns(int line);

	/// <summary>
	/// �w�肵��(�s��,->�̒��̍s��)�̕������Ԃ�
	/// </summary>
	/// <param name="line">�s��</param>
	/// <param name="column">�s��,->�̒��̍s��</param>
	/// <returns>�s��,->�̒��̍s���̕�����</returns>
	std::string GetString(int line, int column);

	/// <summary>
	/// �w�肵��(�s��,->�̒��̍s��)�̃��C�h�������Ԃ�
	/// </summary>
	/// <param name="line">�s��</param>
	/// <param name="column">�s��,->�̒��̍s��</param>
	/// <returns>�s��,->�̒��̍s���̕�����</returns>
	const wchar_t* GetWchar(int line, int column);

	/// <summary>
	/// �w�肵��(�s��,->�̒��̍s��)�̐�����Ԃ�
	/// </summary>
	/// <param name="line">�s��</param>
	/// <param name="column">�s��,->�̒��̍s��</param>
	/// <returns>�s��,->�̒��̍s���̐���</returns>
	int GetInt(int line, int column);

	/// <summary>
	/// �w�肵��(�s��,->�̒��̍s��)�̕��������_��Ԃ�
	/// </summary>
	/// <param name="line">�s��</param>
	/// <param name="column">�s��,->�̒��̍s��</param>
	/// <returns>�s��,->�̒��̍s���̕��������_</returns>
	float GetFloat(int line, int column);

};
