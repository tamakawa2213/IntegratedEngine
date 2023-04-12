#pragma once
#include <vector>

//指定した数だけ関数を呼び出すクラス
class AssignmentFunction
{
	//intをキーとしてその数だけ毎フレーム呼び出す
	std::vector<std::pair<int*, void(AssignmentFunction::*)()>> func_;

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