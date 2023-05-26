#pragma once
#include <string>
#include <utility>

#include "../Json/json.hpp"

namespace JsonOperator
{
	/// <summary>json�t�@�C���̓ǂݍ���</summary>
	/// <param name="filename">�p�X�t�̃t�@�C����</param>
	/// <returns>���� : �g���q���������t�@�C����  ���s : ��̕�����</returns>
	std::string Load(const std::string& filename);

	nlohmann::json GetData(const std::string& filename);
	float GetData(const nlohmann::json& data, const float& Spare, const std::string& key);
	float GetData(const std::string& filename, const float& Spare, const std::string& key);

	template<typename ... Args>
	float GetData(const nlohmann::json& data, const float& Spare, const std::string& key, const Args& ... args)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return GetData(*itr, Spare, std::forward<const std::string>(args)...);
		}

		return Spare;
	}

	template<typename ... Args>
	float GetData(const std::string& filename, const float& Spare, const std::string& key, const Args& ... args)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return GetData(itr.at(key), Spare, std::forward<const std::string>(args)...);
		}

		return Spare;
	}

	bool OverWrite(const std::string& filename, const nlohmann::json& data);
	/*template <typename ... Args>
	bool OverWrite(const std::string& filename, Args ... args)
	{

	}*/
};