#include "JsonOperator.h"
#include <filesystem>
#include <fstream>
#include <memory>

namespace
{
	struct ManagementSet
	{
		std::string FileName;		//�J���t�@�C���̖��O
		nlohmann::json Data;

		ManagementSet() : FileName(), Data() {}
		ManagementSet(const std::string& File, nlohmann::json j) : FileName(File), Data(j) {}

		bool operator ==(const ManagementSet& fl) const { return (FileName == fl.FileName); }
		bool operator !=(const ManagementSet& fl) const { return !(*this == fl); }
	};

	//bool operator ==(const ManagementSet fl, const ManagementSet fr) { return (fl.FileName == fr.FileName); }
	//bool operator !=(const ManagementSet fl, const ManagementSet fr) { return !(fl == fr); }

	bool operator ==(const ManagementSet fl, const std::string str) { return (fl.FileName == str); }
	bool operator !=(const ManagementSet fl, const std::string str) { return !(fl == str); }

	std::vector<ManagementSet> SetList = {};	//�t�@�C�����ƃf�[�^�̃Z�b�g�̔z��
}

namespace JsonOperator
{
	std::string Load(const std::string& filename)
	{
		//�g���q���������t�@�C������؂蔲��
		std::filesystem::path file = filename;

		//���ɓ����t�@�C�����J���Ă���ꍇ
		if (auto itr = std::find(SetList.begin(), SetList.end(), file.stem().string()); itr != end(SetList))
		{
			return file.stem().string();
		}

		//�J���Ă��Ȃ���΁A�ǂݍ���
		std::ifstream ifs;
		ifs.open(filename, std::ios::in);

		//���s�������̕����񂪕Ԃ�
		if (ifs.fail())
		{
			return "";
		}

		std::string s, sData;

		//�t�@�C���̖����܂�1�s���擾
		while (!ifs.eof())
		{
			std::getline(ifs, s);
			sData += s + "\n";
		}
		ifs.close();

		nlohmann::json j;
		j = nlohmann::json::parse(sData);

		ManagementSet List = { file.stem().string(), j};

		SetList.push_back(List);
		return file.stem().string();
	}

	nlohmann::json GetData(const std::string& filename)
	{
		std::filesystem::path file = filename;
		if (auto itr = std::find(SetList.begin(), SetList.end(), file.stem().string()); itr != end(SetList))
		{
			return itr->Data;
		}
		return false;
	}

	bool OverWrite(const std::string& filename, const nlohmann::json& data)
	{
		std::filesystem::path file = filename;

		std::ofstream ofs;
		ofs.open(filename, std::ios::out);

		if (auto itr = std::find(SetList.begin(), SetList.end(), file.stem().string()); itr != end(SetList))
		{
			std::string val = data.dump();
			//itr->Data.push_back(data);

			/*for (auto&& it : data)
			{
				if (auto i = itr->Data.find(it); i != end(itr->Data))
				{
					i = it;
				}
			}*/
			
			for (auto&& str : val)
			{
				if (str == '{' || str == '}' || str == ',')
				{
					ofs << str << std::endl;
				}
				else
				{
					ofs << str;
				}
			}
			ofs.close();
			return true;
		}


		return false;
	}
}