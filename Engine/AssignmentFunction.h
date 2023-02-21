#pragma once
#include <unordered_map>
#include <functional>
#include <map>

//指定した数だけ関数を呼び出すクラス
class AssignmentFunction
{
	//intをキーとしてその数だけ毎フレーム呼び出す
	//std::multimap<void(AssignmentFunction::*)(), int> func_;
	std::unordered_multimap<int, void(AssignmentFunction::*)()> func_;
	//std::map<std::function<void()>, int> func_;

	int Fadeout_;
	void FadeOut();

public:
	//コンストラクタ
	AssignmentFunction();

	//デストラクタ
	~AssignmentFunction();

	//GameObjectクラスで呼び出す
	void Update();

	//フェードアウトにかけるフレーム数を入れる
	void SetFadeout(int frame);
};