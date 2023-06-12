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

	/// <summary>json�t�@�C������l���擾����</summary>
	/// <typeparam name="T">�߂�l�̌^</typeparam>
	/// <typeparam name="...Args">�L�[���̃p�����[�^�p�b�N</typeparam>
	/// <param name="data">�ǂݍ��ޑΏۂ�json�I�u�W�F�N�g</param>
	/// <param name="Spare">�ǂݍ��߂Ȃ������ۂ̒l</param>
	/// <param name="key">�L�[��</param>
	/// <param name="...args">�L�[��</param>
	/// <returns>���� : �ǂݍ��񂾒l  ���s : Spare</returns>
	template<typename T, typename ... Args>
	T GetData(const nlohmann::json& data, const T& Spare, const std::string& key, const Args& ... args)
	{
		if (auto itr = data.find(key); itr != data.end())
		{
			return GetData(*itr, Spare, std::forward<const std::string>(args)...);
		}

		return Spare;
	}

	/// <summary>�t�@�C��������l���擾����</summary>
	/// <typeparam name="T">�߂�l�̌^</typeparam>
	/// <typeparam name="...Args">�L�[���̃p�����[�^�p�b�N</typeparam>
	/// <param name="filename">�ǂݍ��ޑΏۂ̃t�@�C����</param>
	/// <param name="Spare">�ǂݍ��߂Ȃ������ۂ̒l</param>
	/// <param name="key">�L�[��</param>
	/// <param name="...args">�L�[��</param>
	/// <returns>���� : �ǂݍ��񂾒l  ���s : Spare</returns>
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