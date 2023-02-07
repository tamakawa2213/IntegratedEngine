#include "IniOperator.h"
#include <algorithm>
#include <cstring>
#include <Windows.h>

namespace
{
	struct ManagementSet
	{
		std::string FileName;		//�J���t�@�C���̖��O
		std::string SectionName;	//�Z�N�V������
	};
	std::vector<ManagementSet*> SetList = {};	//�t�@�C�����ƃZ�N�V�������̃Z�b�g�̔z��
	std::string KeyName;	//�L�[��
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
			//�t�@�C�����A�Z�N�V�������Ƃ��Ɉ�v���Ă����ꍇ
			if (List->FileName == (*itr).FileName && List->SectionName == (*itr).SectionName)
			{
				//���̔ԍ���Ԃ�
				return (int)std::distance(SetList.front(), itr);
			}
		}

		//�Ȃ���Βǉ�
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