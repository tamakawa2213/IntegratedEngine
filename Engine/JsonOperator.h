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

	template<typename T>
	T GetData(const nlohmann::json& data, const T& Spare, const std::string& key)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return itr->get<T>();
		}

		return Spare;
	}

	template<typename T>
	T GetData(const std::string& filename, const T& Spare, const std::string& key)
	{
		std::filesystem::path file = filename;

		if (auto itr = GetData(file.stem().string()); itr != false)
		{
			return itr.at(key).get<T>();
		}

		return Spare;
	}

	/// <summary>jsonファイルから値を取得する</summary>
	/// <typeparam name="T">戻り値の型</typeparam>
	/// <typeparam name="...Args">キー名のパラメータパック</typeparam>
	/// <param name="data">読み込む対象のjsonオブジェクト</param>
	/// <param name="Spare">読み込めなかった際の値</param>
	/// <param name="key">キー名</param>
	/// <param name="...args">キー名</param>
	/// <returns>成功 : 読み込んだ値  失敗 : Spare</returns>
	template<typename T, typename ... Args>
	T GetData(const nlohmann::json& data, const T& Spare, const std::string& key, const Args& ... args)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return GetData(*itr, Spare, std::forward<const std::string>(args)...);
		}

		return Spare;
	}

	/// <summary>ファイル名から値を取得する</summary>
	/// <typeparam name="T">戻り値の型</typeparam>
	/// <typeparam name="...Args">キー名のパラメータパック</typeparam>
	/// <param name="filename">読み込む対象のファイル名</param>
	/// <param name="Spare">読み込めなかった際の値</param>
	/// <param name="key">キー名</param>
	/// <param name="...args">キー名</param>
	/// <returns>成功 : 読み込んだ値  失敗 : Spare</returns>
	template<typename T, typename ... Args>
	T GetData(const std::string& filename, const T& Spare, const std::string& key, const Args& ... args)
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