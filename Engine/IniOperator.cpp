#include "IniOperator.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <Windows.h>

namespace
{
	struct ManagementSet
	{
		std::string FileName;		//�J���t�@�C���̖��O
		std::string SectionName;	//�Z�N�V������

		ManagementSet() : FileName(), SectionName() {}
		ManagementSet(const std::string& File, const std::string& Section) : FileName(File), SectionName(Section) {}

		bool operator ==(const ManagementSet& fl) const { return (FileName == fl.FileName); }
		bool operator !=(const ManagementSet& fl) const { return !(*this == fl); }
	};

	bool operator ==(const std::shared_ptr<ManagementSet> fl, const std::shared_ptr<ManagementSet> fr) { return (fl->FileName == fr->FileName) && (fl->SectionName == fr->SectionName); }
	bool operator !=(const std::shared_ptr<ManagementSet> fl, const std::shared_ptr<ManagementSet> fr) { return !(fl == fr); }

	std::vector<std::shared_ptr<ManagementSet>> SetList = {};	//�t�@�C�����ƃZ�N�V�������̃Z�b�g�̔z��
}

namespace IniOperator
{
	int AddList(const std::string& filename, const std::string& sectionname)
	{
		std::shared_ptr<ManagementSet> List = std::make_shared<ManagementSet>(filename, sectionname);

		if (auto itr = std::find(SetList.begin(), SetList.end(), List); itr != end(SetList))
		{
			//���̔ԍ���Ԃ�
			return (int)std::distance(SetList.begin(), itr);
		}

		//�Ȃ���Βǉ�
		SetList.push_back(std::move(List));
		return (int)SetList.size() - 1;
	}

	int GetValue(int Listnum, const std::string& Key, int SpareValue)
	{
		return (int)GetPrivateProfileIntA(SetList.at(Listnum)->SectionName.c_str(), Key.c_str(), SpareValue, SetList.at(Listnum)->FileName.c_str());
	}

	void SetValue(int Listnum, const std::string& Key, int Value)
	{
		WritePrivateProfileStringA(SetList.at(Listnum)->SectionName.c_str(), Key.c_str(), std::to_string(Value).c_str(), SetList.at(Listnum)->FileName.c_str());
	}
}; // namespace IniOperator