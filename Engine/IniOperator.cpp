#include "IniOperator.h"
#include <algorithm>
#include <cstring>
#include <Windows.h>

namespace
{
	struct ManagementSet
	{
		std::string FileName;		//開くファイルの名前
		std::string SectionName;	//セクション名
	};
	std::vector<ManagementSet*> SetList = {};	//ファイル名とセクション名のセットの配列
	std::string KeyName;	//キー名
}

namespace IniOperator
{
	int AddList(std::string filename, std::string sectionname)
	{
		ManagementSet* List = new ManagementSet;
		List->FileName = filename;
		List->SectionName = sectionname;
		for (auto itr : SetList)
		{
			//ファイル名、セクション名ともに一致していた場合
			if (List->FileName == (*itr).FileName && List->SectionName == (*itr).SectionName)
			{
				//その番号を返す
				return (int)std::distance(SetList.front(), itr);
			}
		}

		//なければ追加
		SetList.push_back(List);
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