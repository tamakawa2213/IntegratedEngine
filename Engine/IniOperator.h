#pragma once
#include <string>

//Iniファイルの読み込み/書き出しを管理する
namespace IniOperator
{
	/// <summary>ファイル名とセクション名のセットをリストに追加する</summary>
	/// <param name="filename">読み込むファイル名</param>
	/// <param name="sectionname">セクション名</param>
	/// <returns>作成した配列の要素番号</returns>
	int AddList(const std::string& filename, const std::string& sectionname);

	/// <summary>iniファイルから指定したキーの値を取得</summary>
	/// <param name="Listnum">AddListで取得した値</param>
	/// <param name="Key">検索したいキー名</param>
	/// <param name="SpareValue">見つからなかった場合の値</param>
	/// <returns>取得した値</returns>
	int GetValue(int Listnum, const std::string& Key, int SpareValue = -1);

	/// <summary>iniファイルの指定したキーの値を上書き</summary>
	/// <param name="Listnum">AddListで取得した値</param>
	/// <param name="Key">変更したいキー名</param>
	/// <param name="Value">キーに設定する値</param>
	void SetValue(int Listnum, const std::string& Key, int Value);
};