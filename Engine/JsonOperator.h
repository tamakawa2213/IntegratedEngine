#pragma once
#include <string>
#include <utility>

#include "../Json/json.hpp"

namespace JsonOperator
{
	/// <summary>jsonファイルの読み込み</summary>
	/// <param name="filename">パス付のファイル名</param>
	/// <returns>成功 : 拡張子を除いたファイル名  失敗 : 空の文字列</returns>
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