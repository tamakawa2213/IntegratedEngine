#include "IniOperator.h"
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <Windows.h>

namespace
{
	struct ManagementSet
	{
		std::string FileName;		//開くファイルの名前
		std::string SectionName;	//セクション名

		ManagementSet() : FileName(), SectionName() {}
	};
	std::vector<std::shared_ptr<ManagementSet>> SetList = {};	//ファイル名とセクション名のセットの配列
	std::string KeyName;	//キー名
}

namespace IniOperator
{
	int AddList(std::string filename, std::string sectionname)
	{
		std::shared_ptr<ManagementSet> List = std::make_shared<ManagementSet>();
		List->FileName = filename;
		List->SectionName = sectionname;
		if (auto itr = std::find(SetList.begin(), SetList.end(), List); itr != end(SetList))
		{
			//その番号を返す
			return (int)std::distance(SetList.begin(), itr);
		}

		//なければ追加
		SetList.push_back(std::move(List));
		return (int)SetList.size() - 1;
	}

	int GetValue(int Listnum, std::string Key, int SpareValue)
	{
		return (int)GetPrivateProfileIntA(SetList.at(Listnum)->SectionName.c_str(), Key.c_str(), SpareValue, SetList.at(Listnum)->FileName.c_str());
	}

	void SetValue(int Listnum, std::string Key, int Value)
	{
		WritePrivateProfileStringA(SetList.at(Listnum)->SectionName.c_str(), Key.c_str(), std::to_string(Value).c_str(), SetList.at(Listnum)->FileName.c_str());
	}
};