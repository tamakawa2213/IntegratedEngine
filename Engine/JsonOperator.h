#pragma once
#include <string>

#include "../Json/json.hpp"

namespace JsonOperator
{
	/// <summary>jsonファイルの読み込み</summary>
	/// <param name="filename">パス付のファイル名</param>
	/// <returns>成功 : 拡張子を除いたファイル名  失敗 : 空の文字列</returns>
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
	//		return itr[args ...]; //←ここで引数に受け取った数だけ深い階層の値を取得したい
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