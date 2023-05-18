#pragma once
#include <string>

#include "../Json/json.hpp"

namespace JsonOperator
{
	/// <summary>json�t�@�C���̓ǂݍ���</summary>
	/// <param name="filename">�p�X�t�̃t�@�C����</param>
	/// <returns>���� : �g���q���������t�@�C����  ���s : ��̕�����</returns>
	std::string Load(const std::string& filename);

	nlohmann::json GetData(const std::string& filename);

	template<typename T>
	T GetData(const std::string& filename, const std::string& section, const std::string& key)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return itr[section][key];
		}

		return 0;
	}

	//template<typename T, typename ... Args>
	//T GetData(const std::string& filename, Args ... args)
	//{
	//	std::filesystem::path file = filename;

	//	if (auto itr = GetData(file.stem().string()); itr != false)
	//	{
	//		return itr[args ...]; //�������ň����Ɏ󂯎�����������[���K�w�̒l���擾������
	//	}

	//	return 0;
	//}

	template<typename T>
	T GetData(const std::string& filename, const std::string& key)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return itr[key];
		}

		return 0;
	}
	bool OverWrite(const std::string& filename, const nlohmann::json& data);
	/*template <typename ... Args>
	bool OverWrite(const std::string& filename, Args ... args)
	{

	}*/

	
};