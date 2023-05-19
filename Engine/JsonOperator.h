#pragma once
#include <string>
#include <type_traits>

#include "../Json/json.hpp"

namespace JsonOperator
{
	/// <summary>json�t�@�C���̓ǂݍ���</summary>
	/// <param name="filename">�p�X�t�̃t�@�C����</param>
	/// <returns>���� : �g���q���������t�@�C����  ���s : ��̕�����</returns>
	std::string Load(const std::string& filename);

	nlohmann::json GetData(const std::string& filename);

	template<class T, class ... Args>
	T GetData(const nlohmann::json& data, const T& Spare, const std::string& key, const Args& ... args)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return GetData<T>(itr->at(key), Spare, args ...);
		}

		return Spare;
	}

	template<class T>
	T GetData(const nlohmann::json& data, const T& Spare, const std::string& key)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return itr->get<T>();
		}

		return Spare;
	}

	template<class T, class ... Args>
	T GetData(const std::string& filename, const T& Spare, const std::string& key, const Args& ... args)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return GetData<T>(itr.at(key), Spare, args ...);
		}

		return Spare;
	}

	template<class T>
	T GetData(const std::string& filename, const T& Spare, const std::string& key)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return itr.at(key).get<T>();
		}

		return Spare;
	}

	bool OverWrite(const std::string& filename, const nlohmann::json& data);
	/*template <typename ... Args>
	bool OverWrite(const std::string& filename, Args ... args)
	{

	}*/

	
};